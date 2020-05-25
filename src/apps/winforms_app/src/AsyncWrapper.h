#include <ann/iann_controller.h>

namespace annWinForm {

using ManagedConfusionMatrix = array<array<System::Int32>^>^ ;

ref class AsyncWrapper{

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
};

}