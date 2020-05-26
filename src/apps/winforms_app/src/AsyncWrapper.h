#include <ann/iann_controller.h>

namespace annWinForm {

using ManagedConfusionMatrix = array<array<System::Int32>^>^;

ref struct TrainSnapshot {

    TrainSnapshot(int itemCount)
    {
        mImagePrediction = gcnew TrainSnapshot::IntArray(itemCount);
        mImageIndex = gcnew TrainSnapshot::IntArray(itemCount);
        mImageLabel = gcnew TrainSnapshot::IntArray(itemCount);
    }
    using IntArray = array<System::Int32>;

    IntArray^ mImageIndex;
    IntArray^ mImagePrediction;
    IntArray^ mImageLabel;
};

delegate void confusionMatrixArrived(ManagedConfusionMatrix);
delegate void testStatusUpdate(int numberOfImages);
delegate void trainSnapshotUpdate(TrainSnapshot^ snapshot);

interface struct IWrapperEvents{
    event confusionMatrixArrived^ OnConfusionMatrixArrived;
    event testStatusUpdate^ OnTestStatusUpdate;
    event trainSnapshotUpdate^ OnTrainSnapshotUpdate;
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
    void createSnapshot(void);
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

    System::Drawing::Bitmap^ getBitmap(int imageNumber);

    virtual event confusionMatrixArrived^ OnConfusionMatrixArrived;
    virtual event testStatusUpdate^ OnTestStatusUpdate;
    virtual event trainSnapshotUpdate^ OnTrainSnapshotUpdate;
};

}