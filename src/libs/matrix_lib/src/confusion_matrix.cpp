#include <matrix/confusion_matrix.h>
#include <sstream>
#include <iomanip>

namespace math
{
    ConfusionMatrix::ConfusionMatrix(size_t classCount)
    {
        mData = math::make_matrix<size_t>(classCount, classCount);
    }


    void ConfusionMatrix::update(const math::MatrixF &predictions, const math::MatrixF &labels)
    {
        BOOST_ASSERT(predictions.getColumnCount() == labels.getColumnCount());
        BOOST_ASSERT(predictions.getRowCount() == labels.getRowCount());
        BOOST_ASSERT(predictions.getRowCount() == mData.getRowCount());

        for (size_t i = 0; i < predictions.getColumnCount(); ++i)
        {
            auto max_prediction_it = std::max_element(predictions.cColumnBegin(i), predictions.cColumnEnd(i));
            auto max_label_it = std::max_element(labels.cColumnBegin(i), labels.cColumnEnd(i));

            size_t prediction = std::distance(predictions.cColumnBegin(i), max_prediction_it);
            size_t label = std::distance(labels.cColumnBegin(i), max_label_it);

            BOOST_ASSERT(prediction < mData.getColumnCount());
            BOOST_ASSERT(label < mData.getColumnCount());

            mData.set(prediction, label, mData.get(prediction, label) + 1);
        }
    }

    std::string ConfusionMatrix::toString() const
    {
        std::stringstream out;
        for (size_t i = 0; i < mData.getRowCount(); ++i)
        {
            for (size_t j = 0; j < mData.getColumnCount(); ++j)
            {
                out << std::setw(6) << std::setfill(' ')  << mData.get(i, j) << ", ";
            }
            out << '\n';
        }
        return out.str();
    }
}