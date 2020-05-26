#include "async_loop.h"
#include "ann.h"
#include "shuffled_range.h"
#include "utils.h"
#include <matrix/confusion_matrix.h>

namespace ann { namespace async {

ThreadCommandQueue::message::buffer serializeConfusionMatrix(math::ConfusionMatrix::internalMatrix &matrix);
ThreadCommandQueue::message::buffer serializeTestStatus(size_t numImages);


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

    std::cout << "starting message loop on the background thread" << std::endl;

    while (mInternalState != InternalState::QUIT)
    {
        // get command
        auto result = mIncomingQueue->getCommand(mInternalState == InternalState::IDLE);
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

                        MainCommandQueue::message trainStatusMsg{
                            commandToMain::TESTING_PROGRESS_STATUS,
                            serializeTestStatus((testState->mCurrentIteration) * 50)
                        };
                        mResultQueue->pushCommand(trainStatusMsg);

                        if (!hasMoreElements)
                        {
                            mNetworkState = NetworkState::NONE;
                            mInternalState = InternalState::IDLE;
                            std::cout << testState->mConfusionMatrix.toString() << std::endl;

                            MainCommandQueue::message message {
                                commandToMain::TESTING_FINISHED,
                                serializeConfusionMatrix(testState->mConfusionMatrix.getMatrix())
                            };

                            mResultQueue->pushCommand(message);

                        }
                    }
                    break;

            }
        }
    }
    std::cout << "worker thread finished" << std::endl;
}

ThreadCommandQueue::message::buffer serializeConfusionMatrix(math::ConfusionMatrix::internalMatrix &matrix)
{
    ThreadCommandQueue::message::buffer result;
    const uint32_t rowCount = static_cast<uint32_t>(matrix.getRowCount());
    result.resize(sizeof(rowCount) * (1 + rowCount * rowCount));

    auto offset = result.data();
    std::memcpy(offset, &rowCount, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    for (auto value : matrix)
    {
        std::memcpy(offset, &static_cast<uint32_t>(value), sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
    return result;
}

ThreadCommandQueue::message::buffer serializeTestStatus(size_t numImages)
{
    ThreadCommandQueue::message::buffer result;
    result.resize(sizeof(uint32_t));
    std::memcpy(result.data(), &static_cast<uint32_t>(numImages), sizeof(uint32_t));
    return result;
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
    const size_t batchSize = 50;

    std::cout << state.mCurrentIteration<< ": " << state.mLastLoss << std::endl;

    state.mCurrentRange = generateNewRange(*state.mRangeGenerator, batchSize);
    state.mImages = getImageDataBasedOnRange(*mTrainDb, state.mCurrentRange);
    state.mLabels = getImageClassBasedOnRange(*mTrainDb, state.mCurrentRange);

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