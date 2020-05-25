#include "AsyncWrapper.h"
#include <msclr/marshal_cppstd.h>
#include <db/idb.h>


namespace annWinForm
{

using namespace System;

ManagedConfusionMatrix deserializeConfusionMatrix(const ann::async::MainMessage::buffer &buffer);
int deserializeImageCount(const ann::async::MainMessage::buffer& buffer);

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

void AsyncWrapper::checkMessage(void)
{
    auto result = mController->getAsyncCommand();
    while (result)
    {
        switch (result->mCommand)
        {
        case ann::async::commandToMain::TESTING_FINISHED:
            {
                auto confusionMatrix = deserializeConfusionMatrix(result->mBuffer);
                OnConfusionMatrixArrived(confusionMatrix);
            }
            break;

        case ann::async::commandToMain::TESTING_PROGRESS_STATUS:
            {
                auto processedImageCount = deserializeImageCount(result->mBuffer);
                OnTestStatusUpdate(processedImageCount);
            }
            break;
        }

        result = mController->getAsyncCommand();
    }
}

int deserializeImageCount(const ann::async::MainMessage::buffer &buffer)
{
    int32_t data;
    std::memcpy(&data, buffer.data(), sizeof(uint32_t));
    return data;
}


ManagedConfusionMatrix deserializeConfusionMatrix(const ann::async::MainMessage::buffer &buffer)
{
    uint32_t rowCount = 0;
    auto offset = buffer.data();

    std::memcpy(&rowCount, offset, sizeof(rowCount));
    offset += sizeof(rowCount);

    ManagedConfusionMatrix result = gcnew array<array< Int32 >^>(rowCount);
    for (uint32_t i = 0; i < rowCount; ++i)
    {
        result[i] = gcnew array< Int32 >(rowCount);
        for (uint32_t j = 0; j < rowCount; ++j)
        {
            uint32_t data;
            std::memcpy(&data, offset, sizeof(data));
            offset += sizeof(data);
            result[i][j] = data;
        }
    }
    return result;
}


}
