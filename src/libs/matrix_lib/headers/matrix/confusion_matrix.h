#pragma once
#include <matrix/matrix.h>


namespace math {

class ConfusionMatrix
{
    using element_type = size_t;
    using internalMatrix = math::Matrix<element_type>;
    internalMatrix mData;

public:
    ConfusionMatrix(size_t classCount);

    void update(const math::MatrixF &predictions, const math::MatrixF &labels);
    std::string toString() const;
};

}