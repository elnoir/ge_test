#pragma once
#include <db/idb.h>
#include <ann/iann_controller.h>
#include <thread>
#include "async_command_defs.h"

namespace ann { namespace async {

class AsyncController
    : public IANNController
{
    db::DBInterfacePtr mTrainDb;
    db::DBInterfacePtr mTestDb;

    ThreadCommandQueuePtr mThreadQueue;
    MainCommandQueuePtr mThreadResult;

    std::thread mAnnThread;

public:
    bool setTrainDb(db::DBInterfacePtr dbPtr) override;
    bool setTestDb(db::DBInterfacePtr dbPtr) override;
    db::DBInterfacePtr getTrainDb() override;
    bool configureNetwork() override;

    bool startTraining() override;
    bool getTrainingSnapshot() override;
    bool pauseTraining() override;
    bool stopTraining() override;

    bool startTest() override;
    bool stopTest() override;

    ann::IANNController::Status getStatus() override;

    virtual ann::async::MainOptMessage getAsyncCommand() override;

    virtual ~AsyncController()
    {
        if (mAnnThread.joinable())
        {
            mThreadQueue->pushCommand({commandToThread::STOP, {}});
            mAnnThread.join();
        }
    }


};

}} // namespace
