#include <ann/iann_controller.h>
#include "ann.h"

namespace ann { namespace sync {

class SyncController
    : public IANNController
{
    db::DBInterfacePtr mTrainDb;
    db::DBInterfacePtr mTestDb;
    std::unique_ptr<ANN> mNetwork;

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
    ann::async::MainOptMessage getAsyncCommand() override;

    virtual ~SyncController() = default;
};



} } // namespace