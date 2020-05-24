#pragma once
#include <matrix/matrix.h>

namespace ann {

class IANN
{
public:
    virtual void configureNetwork(size_t inputSize) = 0;
    virtual float train(const math::MatrixF &inputs, const math::MatrixF &expectedOutput) = 0;
    virtual const math::MatrixF& test(const math::MatrixF &input) = 0;
    virtual ~IANN() = default;
};

}
