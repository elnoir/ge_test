#include <ann/iann_controller.h>

namespace annWinForm {

using ManagedConfusionMatrix = array<array<System::Int32>^>^ ;

delegate void confusionMatrixArrived(ManagedConfusionMatrix);

interface struct IWrapperEvents{
    event confusionMatrixArrived^ OnConfusionMatrixArrived;

};

ref class AsyncWrapper
    : public IWrapperEvents
{
    ann::IANNController *mController;

public:
    AsyncWrapper();

    bool setDb(System::String^ trainImage, System::String^ trainLabel, System::String^ testImage, System::String^ testLabel);

    void configureNetwork(void);
    void startTraining(void);
    void stopTraining(void);
    void startTest(void);
    void stopTest(void);

    void checkMessage(void);

    ~AsyncWrapper()
    {
        this->!AsyncWrapper();
    }

    !AsyncWrapper()
    {
        delete mController;
    }

    virtual event confusionMatrixArrived^ OnConfusionMatrixArrived;
};

}