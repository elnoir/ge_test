#pragma once
#include <db/idb.h>
#include <boost/optional.hpp>
#include "async_command.h"

namespace ann {

enum class ControllerType {
    SYNC,
    ASYNC
};

class IANNController
{
public:
    enum class Status {
        NONE,
        EMPTY,
        CONFIGURED,
        TRAINING_IN_PROGRESS,
        TRAINING_PAUSED,
        TRAINING_DONE,
        TESTING,
        TEST_FINISHED
    };

    virtual bool setTrainDb(db::DBInterfacePtr dbPtr) = 0;
    virtual bool setTestDb(db::DBInterfacePtr dbPtr) = 0;

    virtual bool configureNetwork() =0;

    virtual bool startTraining() = 0;
    virtual bool pauseTraining() = 0;
    virtual bool stopTraining() = 0;

    virtual bool startTest() = 0;
    virtual bool stopTest() = 0;

    virtual ann::IANNController::Status getStatus() = 0;
    virtual ann::async::MainOptMessage getAsyncCommand() = 0;

    virtual ~IANNController() = default;
};

using IANNControllerPtr = std::unique_ptr<ann::IANNController>;

IANNControllerPtr createController(ControllerType t);

}
