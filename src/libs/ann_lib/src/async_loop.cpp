#include <boost/range/algorithm/for_each.hpp>
#include "async_loop.h"
#include "ann.h"
#include "shuffled_range.h"
#include "utils.h"
#include <matrix/confusion_matrix.h>

namespace ann { namespace async {

ThreadCommandQueue::message::buffer serializeConfusionMatrix(const math::ConfusionMatrix::internalMatrix &matrix);
ThreadCommandQueue::message::buffer serializeTestStatus(const size_t numImages);
ThreadCommandQueue::message::buffer serializeTrainStatus(const IANN::trainData &trainData);


struct TrainState
{
    std::unique_ptr<ShuffledRange> mRangeGenerator;
    ShuffledRange::IndexedRange mCurrentRange;

    math::MatrixF mImages;
    math::MatrixF mLabels;

    float mLastLoss = 0.0f;

    size_t mCurrentIteration = 0;

    bool mCreateSnapshot = false;
};

struct TestState
{
    std::unique_ptr<ShuffledRange> mRangeGenerator;
    ShuffledRange::IndexedRange mCurrentRange;
    math::ConfusionMatrix mConfusionMatrix = math::ConfusionMatrix(10);

    size_t mCurrentIteration = 0;
};

AsyncLoop::AsyncLoop(ThreadCommandQueuePtr threadCommand, MainCommandQueuePtr mainCommand, db::DBInterfacePtr trainDb, db::DBInterfacePtr testDb)
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
                case commandToThread::GET_TRAINING_SNAPSHOT:
                    if (trainState)
                    {
                        trainState->mCreateSnapshot = true;
                    }
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
                    if (trainState->mCreateSnapshot)
                    {
                        auto trainResult = mNetwork->collectTrainData();
                        boost::for_each(trainState->mCurrentRange, [&trainResult](size_t range) {
                            trainResult.push_back(static_cast<uint32_t>(range));
                        });
                        MainCommandQueue::message snapshotMessge{
                            commandToMain::TRAINING_SNAPSHOT,
                            serializeTrainStatus(trainResult)
                        };
                        mResultQueue->pushCommand(snapshotMessge);

                        trainState->mCreateSnapshot = false;
                    }
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

ThreadCommandQueue::message::buffer serializeConfusionMatrix(const math::ConfusionMatrix::internalMatrix &matrix)
{
    ThreadCommandQueue::message::buffer result;
    const uint32_t rowCount = static_cast<uint32_t>(matrix.getRowCount());
    result.resize(sizeof(rowCount) * (1 + rowCount * rowCount));

    auto offset = result.data();
    std::memcpy(offset, &rowCount, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    for (auto value : matrix)
    {
        auto v = static_cast<uint32_t>(value);
        std::memcpy(offset, &v, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
    return result;
}

ThreadCommandQueue::message::buffer serializeTestStatus(const size_t numImages)
{
    ThreadCommandQueue::message::buffer result;
    result.resize(sizeof(uint32_t));
    auto v = static_cast<uint32_t>(numImages);
    std::memcpy(result.data(), &v, sizeof(uint32_t));
    return result;
}

ThreadCommandQueue::message::buffer serializeTrainStatus(const IANN::trainData &trainData)
{
    ThreadCommandQueue::message::buffer result;

    result.resize(trainData.size() * sizeof(uint32_t));
    auto offset = result.data();
    boost::for_each(trainData, [&offset](uint32_t value){
        std::memcpy(offset, &value, sizeof(value));
        offset += sizeof(value);
    });

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