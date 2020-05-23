#include "sync_controller.h"
#include "shuffled_range.h"
#include "utils.h"
#include <iostream>
#include <matrix/confusion_matrix.h>


namespace ann {

bool SyncController::setTrainDb(db::DBInterfacePtr dbPtr)
{
    BOOST_ASSERT(dbPtr->isDbLoaded());
    mTrainDb = std::move(dbPtr);
    return true;
}

bool SyncController::setTestDb(db::DBInterfacePtr dbPtr)
{
    BOOST_ASSERT(dbPtr->isDbLoaded());
    mTestDb = std::move(dbPtr);
    return true;
}

bool SyncController::startTraining()
{
    const size_t rangeSize = 50;
    const auto costLimit = 0.03f;

    BOOST_ASSERT(mTrainDb->isDbLoaded());
    mNetwork.configureNetwork(mTrainDb->getPixelCount());
    ShuffledRange rangeGenerator(mTrainDb->getImageCount());
    size_t trainCount = 0;

    float cost = 100.0f;
    while (rangeGenerator.hasMoreElements() && costLimit < cost)
    {
        auto range = rangeGenerator.getNextN(rangeSize);
        auto images = getImageDataBasedOnRange(*mTrainDb, range);
        auto labels = getImageClassBasedOnRange(*mTrainDb, range);
        for (size_t i=0; i < 15 && costLimit < cost; ++i)
        {
            cost = mNetwork.train(images, labels);
            std::cout << "cost: " << cost << std::endl;
        }
        trainCount += rangeSize;
        std::cout << trainCount << "/" << mTrainDb->getImageCount() << std::endl;
    }
    rangeGenerator.shuffle();

    return true;
}

bool SyncController::pauseTraining()
{
    return true;
}

bool SyncController::stopTraining()
{
    return true;
}

bool SyncController::startTest()
{
    const size_t rangeSize = 1000;
    const size_t batchSize = 50;

    BOOST_ASSERT(mTestDb->isDbLoaded());
    ShuffledRange rangeGenerator(mTestDb->getImageCount());
    math::ConfusionMatrix confusion(10);

    for (size_t i = 0; i < rangeSize; i += batchSize)
    {
        auto range = rangeGenerator.getNextN(batchSize);
        auto images = getImageDataBasedOnRange(*mTestDb, range);
        auto labels = getImageClassBasedOnRange(*mTestDb, range);

        const auto& result = mNetwork.test(images);
        confusion.update(result, labels);
        std::cout << "tested on: " << i << std::endl;
    }
    std::cout << confusion.toString() << std::endl;

    return true;
}

bool SyncController::stopTest()
{
    return true;
}

}