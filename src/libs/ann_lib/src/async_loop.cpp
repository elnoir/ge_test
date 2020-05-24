#include "async_loop.h"
#include "ann.h"
#include "shuffled_range.h"
#include "utils.h"
#include <matrix/confusion_matrix.h>

namespace ann { namespace async {


struct TrainState
{
    std::unique_ptr<ShuffledRange> mRangeGenerator;
    ShuffledRange::IndexedRange mCurrentRange;

    math::MatrixF mImages;
    math::MatrixF mLabels;

    float mLastLoss = 0.0f;

    size_t mCurrentIteration = 0;
};

struct TestState
{
    std::unique_ptr<ShuffledRange> mRangeGenerator;
    ShuffledRange::IndexedRange mCurrentRange;
    math::ConfusionMatrix mConfusionMatrix = math::ConfusionMatrix(10);

    size_t mCurrentIteration = 0;
};

AsyncLoop::AsyncLoop(ThreadCommandQueuePtr threadCommand, MainCommandQueuePtr mainCommand, SharedDbPtr trainDb, SharedDbPtr testDb)
    : mTrainDb(trainDb)
    , mTestDb(testDb)
    , mIncomingQueue(threadCommand)
    , mResultQueue(mainCommand)
{
}

void AsyncLoop::Run()
{
    std::unique_ptr<TrainState> trainState;
    std::unique_ptr<TestState> testState;

    while (mInternalState != InternalState::QUIT)
    {
        // get command
        auto result = mIncomingQueue->getCommand();
        if (result)
        {
            switch (result->mCommand)
            {
                case commandToThread::CONFIGURE_NETWORK:
                    mNetwork = std::make_unique<ANN>();
                    mNetwork->configureNetwork(mTrainDb->getPixelCount());

                    trainState = std::make_unique<TrainState>();
                    trainState->mRangeGenerator.reset( new ShuffledRange(mTrainDb->getImageCount()) );

                    mInternalState = InternalState::IDLE;
                    break;
                case commandToThread::GO_IDLE:
                    mInternalState = InternalState::IDLE;
                    break;
                case commandToThread::STOP:
                    mInternalState = InternalState::QUIT;
                    break;
                case commandToThread::START_TRAINING:
                    mInternalState = InternalState::BUSY;
                    mNetworkState = NetworkState::TRAIN;
                    break;
                case commandToThread::START_TESTING:
                    mInternalState = InternalState::BUSY;
                    mNetworkState = NetworkState::RECOGNIZE;

                    testState = std::make_unique<TestState>();
                    testState->mRangeGenerator.reset( new ShuffledRange( mTestDb->getImageCount()) );
                    break;
            }
        }

        if (mInternalState == InternalState::BUSY)
        {
            switch(mNetworkState)
            {
                case NetworkState::TRAIN:
                    nextTrainStep(*mNetwork, *trainState);
                    break;
                case NetworkState::RECOGNIZE:
                    {
                        auto hasMoreElements = nextTestStep(*mNetwork, *testState);
                        if (!hasMoreElements)
                        {
                            mNetworkState = NetworkState::NONE;
                            mInternalState = InternalState::IDLE;
                            std::cout << testState->mConfusionMatrix.toString() << std::endl;
                        }
                    }
                    break;

            }
        }
    }
}


ShuffledRange::IndexedRange generateNewRange(ShuffledRange &generator, size_t count)
{
    if (!generator.hasMoreElements())
    {
        generator.shuffle();
    }
    auto result = generator.getNextN(count);
    return result;
}


void AsyncLoop::nextTrainStep(ann::IANN &network, TrainState& state)
{
    const size_t iterLimit = 15;
    const size_t imageCount = 50;

    if ((state.mCurrentIteration % iterLimit) == 0)
    {
        std::cout << state.mCurrentIteration<< ": " << state.mLastLoss << std::endl;

        state.mCurrentRange = generateNewRange(*state.mRangeGenerator, imageCount);
        state.mImages = getImageDataBasedOnRange(*mTrainDb, state.mCurrentRange);
        state.mLabels = getImageClassBasedOnRange(*mTrainDb, state.mCurrentRange);
    }
    state.mLastLoss = mNetwork->train(state.mImages, state.mLabels);
    state.mCurrentIteration += 1;
}


bool AsyncLoop::nextTestStep(ann::IANN &network, TestState& state)
{
    const size_t batchSize = 50;
    const size_t targetCount = 1000;

    state.mCurrentRange = generateNewRange(*state.mRangeGenerator, batchSize);
    auto images = getImageDataBasedOnRange(*mTestDb, state.mCurrentRange);
    auto labels = getImageClassBasedOnRange(*mTestDb, state.mCurrentRange);

    const auto& result = mNetwork->test(images);
    state.mConfusionMatrix.update(result, labels);
    state.mCurrentIteration += 1;

    return state.mCurrentIteration * batchSize < targetCount;
}

} } // namespace