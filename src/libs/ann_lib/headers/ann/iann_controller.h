#pragma once
#include <db/idb.h>

namespace ann {

enum class ControllerType {
    SYNC,
    ASYNC
};

class IANNController
{
public:
    virtual bool setTrainDb(db::DBInterfacePtr dbPtr) = 0;
    virtual bool setTestDb(db::DBInterfacePtr dbPtr) = 0;

    virtual bool startTraining() = 0;
    virtual bool pauseTraining() = 0;
    virtual bool stopTraining() = 0;

    virtual bool startTest() = 0;
    virtual bool stopTest() = 0;

    virtual ~IANNController() = default;
};

using IANNControllerPtr = std::unique_ptr<ann::IANNController>;

IANNControllerPtr createController(ControllerType t);

}
