#pragma once
#include "async_command_defs.h"
#include <ann/iann.h>

namespace ann { namespace async {

struct TrainState;
struct TestState;

class AsyncLoop
{
    db::DBInterfacePtr mTrainDb;
    db::DBInterfacePtr mTestDb;

    ThreadCommandQueuePtr mIncomingQueue;
    MainCommandQueuePtr mResultQueue;

    enum class InternalState{
        BUSY,
        IDLE,
        QUIT
    };

    enum class NetworkState {
        NONE,
        TRAIN,
        RECOGNIZE,
    };


    InternalState mInternalState = InternalState::IDLE;
    NetworkState mNetworkState = NetworkState::NONE;

    std::unique_ptr<ann::IANN> mNetwork;

public:
    AsyncLoop(ThreadCommandQueuePtr threadCommand, MainCommandQueuePtr mainCommand, db::DBInterfacePtr trainDb, db::DBInterfacePtr testDb);
    void Run();

private:
    void nextTrainStep(ann::IANN &network, TrainState&);
    bool nextTestStep(ann::IANN &network, TestState&);

};


}}
