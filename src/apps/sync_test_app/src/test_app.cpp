#include <ann/iann_controller.h>

int main(int argc, char* params[])
{
    auto controller = ann::createController(ann::ControllerType::SYNC);
    auto db = db::createDB();
    auto dbOpenResult = db->loadDB("data\\train-images.idx3-ubyte", "data\\train-labels.idx1-ubyte");
    BOOST_ASSERT(dbOpenResult);

    auto testDb = db::createDB();
    auto testOpenResult = testDb->loadDB("data\\t10k-images.idx3-ubyte", "data\\t10k-labels.idx1-ubyte");
    BOOST_ASSERT(testOpenResult);

    controller->setTrainDb(std::move(db));
    controller->setTestDb(std::move(testDb));
    controller->startTraining();
    controller->startTest();



}