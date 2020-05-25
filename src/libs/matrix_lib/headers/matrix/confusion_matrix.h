#pragma once
#include <matrix/matrix.h>


namespace math {

class ConfusionMatrix
{
public:
    using element_type = size_t;
    using internalMatrix = math::Matrix<element_type>;
private:
    internalMatrix mData;

public:
    ConfusionMatrix(size_t classCount);

    void update(const math::MatrixF &predictions, const math::MatrixF &labels);
    ConfusionMatrix::internalMatrix getMatrix() const;
    std::string toString() const;
};


}