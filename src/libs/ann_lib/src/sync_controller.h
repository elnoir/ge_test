#include <ann/iann_controller.h>
#include "ann.h"

namespace ann {

class SyncController
    : public IANNController
{
    db::DBInterfacePtr mTrainDb;
    db::DBInterfacePtr mTestDb;
    ANN mNetwork;

public:
    bool setTrainDb(db::DBInterfacePtr dbPtr) override;
    bool setTestDb(db::DBInterfacePtr dbPtr) override;

    bool startTraining() override;
    bool pauseTraining() override;
    bool stopTraining() override;

    bool startTest() override;
    bool stopTest() override;

    virtual ~SyncController() = default;
};



}