#include <ann/iann_controller.h>

namespace annWinForm {

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