#include <ann/iann.h>
#include "async_controller.h"
#include "async_loop.h"

namespace ann{ namespace async {

    void startCommandLoop(SharedDbPtr trainDb, SharedDbPtr testDb, ThreadCommandQueuePtr threadCommandsPtr, MainCommandQueuePtr mainCommandPtr)
    {
        AsyncLoop loopHandler(threadCommandsPtr, mainCommandPtr, trainDb, testDb);
        loopHandler.Run();
    }


    bool AsyncController::setTrainDb(db::DBInterfacePtr dbPtr)
    {
        BOOST_ASSERT(dbPtr->isDbLoaded());
        mTrainDb = std::move(dbPtr);
        return true;
    }

    bool AsyncController::setTestDb(db::DBInterfacePtr dbPtr)
    {
        BOOST_ASSERT(dbPtr->isDbLoaded());
        mTestDb = std::move(dbPtr);
        return true;
    }

    bool AsyncController::configureNetwork()
    {
        BOOST_ASSERT(mTestDb && mTestDb->isDbLoaded());
        BOOST_ASSERT(mTrainDb && mTrainDb->isDbLoaded());
        bool isDbOk = mTestDb && mTestDb->isDbLoaded() && mTrainDb && mTrainDb->isDbLoaded();
        if (isDbOk && !mAnnThread.joinable())
        {
            mThreadQueue = std::make_shared<ThreadCommandQueuePtr::element_type>();
            mThreadResult = std::make_shared<MainCommandQueuePtr::element_type>();
            mAnnThread = std::thread(startCommandLoop, mTrainDb, mTestDb, mThreadQueue, mThreadResult);
        }

        mThreadQueue->pushCommand({commandToThread::CONFIGURE_NETWORK, {}});
        return isDbOk;
    }

    bool AsyncController::startTraining()
    {
        mThreadQueue->pushCommand({commandToThread::START_TRAINING, {}});
        return true;
    }

    bool AsyncController::pauseTraining()
    {
        mThreadQueue->pushCommand({commandToThread::GO_IDLE, {}});
        return true;
    }

    bool AsyncController::stopTraining()
    {
        mThreadQueue->pushCommand({commandToThread::GO_IDLE, {}});
        return true;
    }

    bool AsyncController::startTest()
    {
        mThreadQueue->pushCommand({commandToThread::START_TESTING, {}});
        return true;
    }

    bool AsyncController::stopTest()
    {
        mThreadQueue->pushCommand({commandToThread::GO_IDLE, {}});
        return true;
    }

    ann::IANNController::Status AsyncController::getStatus()
    {
        return ann::IANNController::Status::EMPTY;
    }

}} // namespace