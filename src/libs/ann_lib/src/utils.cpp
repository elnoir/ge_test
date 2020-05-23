#include "ann/iann_controller.h"
#include "sync_controller.h"
#include "shuffled_range.h"
#include <boost/range/algorithm/copy.hpp>
#include "utils.h"

namespace ann {

IANNControllerPtr createController(ControllerType t)
{
    std::unique_ptr<IANNController> result;
    switch (t)
    {
    case ControllerType::SYNC:
        result.reset(new sync::SyncController());
        break;

    case ControllerType::ASYNC:
        break;
    }

    return result;
}


math::MatrixF getImageDataBasedOnRange(const db::DBInterface &db, const ShuffledRange::IndexedRange &range)
{
    const auto rowCount = db.getPixelCount();
    auto result = math::make_matrix<float>(rowCount, range.size());

    for (size_t i = 0; i < range.size(); ++i)
    {
        auto image = db.getImageMatrix(range[i]);
        boost::copy(image, result.columnBegin(i));
    }
    return result;
}

math::MatrixF getImageClassBasedOnRange(const db::DBInterface &db, const ShuffledRange::IndexedRange &range)
{
    const auto rowCount = 10;
    auto result = math::make_matrix<float>(rowCount, range.size());

    for (size_t i = 0; i < range.size(); ++i)
    {
        auto label = db.getImageLabel(range[i]);
        boost::copy(label, result.columnBegin(i));
    }
    return result;
}


}