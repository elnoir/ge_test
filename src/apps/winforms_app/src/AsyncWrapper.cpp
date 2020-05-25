#include "AsyncWrapper.h"
#include <msclr/marshal_cppstd.h>
#include <db/idb.h>


namespace annWinForm
{


AsyncWrapper::AsyncWrapper()
{
    mController = ann::createController(ann::ControllerType::ASYNC).release();
}

bool AsyncWrapper::setDb(System::String^ trainImage, System::String^ trainLabel, System::String^ testImage, System::String^ testLabel)
{
    System::Console::WriteLine("SetDB Called");
    std::string sTrainImage = msclr::interop::marshal_as<std::string>(trainImage);
    std::string sTrainLabel = msclr::interop::marshal_as<std::string>(trainLabel);
    std::string sTestImage = msclr::interop::marshal_as<std::string>(testImage);
    std::string sTestLabel = msclr::interop::marshal_as<std::string>(testLabel);

    auto trainDb = db::createDB();
    bool db1_result = trainDb->loadDB(sTrainImage, sTrainLabel);
    auto testDb = db::createDB();
    bool db2_result = testDb->loadDB(sTestImage, sTestLabel);

    if (db1_result && db2_result)
    {
        System::Console::WriteLine("DB load Ok");
        mController->setTrainDb(std::move(trainDb));
        mController->setTestDb(std::move(testDb));
    }

    return db1_result && db2_result;

}

void AsyncWrapper::configureNetwork(void)
{
    mController->configureNetwork();
}

void AsyncWrapper::startTraining(void)
{
    mController->startTraining();
}

void AsyncWrapper::stopTraining(void)
{
    mController->stopTraining();
}

void AsyncWrapper::startTest(void)
{
    mController->startTest();

}

void AsyncWrapper::stopTest(void)
{
    mController->stopTest();
}



}