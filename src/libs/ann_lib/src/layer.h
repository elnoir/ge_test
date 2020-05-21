#pragma once

#include <matrix/matrix.h>
#include <functional>

namespace ann {

class Layer
{
public:
    using layerFx = std::function<math::MatrixF(const math::MatrixF&)>;

private:
    math::MatrixF mInput;
    math::MatrixF mWeights;
    math::MatrixF mBias;
    math::MatrixF mZ;
    math::MatrixF mActivation;

    layerFx mForwardFunc;
    layerFx mDerivateFunc;

    math::MatrixF mWeightDelta;
    math::MatrixF mBiasDelta;

public:
    Layer(size_t inputRowCount, size_t outputRowCount, layerFx forwardFunc, layerFx derivateFunc);

    math::MatrixF feedForward(const math::MatrixF &inputData);
    const math::MatrixF& getResult() const { return mActivation; }

    math::MatrixF beginBackPropagation(const math::MatrixF &expectedOutput);
    math::MatrixF calculateGradients(const math::MatrixF &prevDz);
    void applyGradients(float learnSpeed);
};

}