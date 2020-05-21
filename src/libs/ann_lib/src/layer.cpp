#include "layer.h"

#include <random>
#include <cmath>
#include <boost/range/algorithm/fill.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace ann {

Layer::Layer(size_t inputRowCount, size_t outputRowCount, layerFx forwardFunc, layerFx derivateFunc)
    : mForwardFunc(forwardFunc)
    , mDerivateFunc(derivateFunc)
{
    mWeights = math::make_matrix<float>(outputRowCount, inputRowCount);
    mBias = math::make_matrix<float>(outputRowCount, 1);

    std::random_device rd{};
    std::mt19937 generator{rd()};
    std::normal_distribution<float> distribution{0.0f, 1.0f};

    const auto div = std::sqrtf(static_cast<float>(inputRowCount / 2));
    boost::for_each(mWeights, [&generator, &distribution, &div](float &value) {
        value = distribution(generator) / div;
    });
    boost::fill(mBias, 0.01f);
}

math::MatrixF Layer::feedForward(const math::MatrixF &inputData)
{
    mInput = inputData;
    mZ = (mWeights * mInput).addColumnVector(mBias);
    mActivation = mForwardFunc(mZ);
    return mActivation;
}

math::MatrixF Layer::beginBackPropagation(const math::MatrixF &expectedOutput)
{
    const auto m = static_cast<float>(mInput.getColumnCount());

    auto lossd = mActivation - expectedOutput;
    mWeightDelta = (1.0f / m) * (lossd * mInput.transpose());
    mBiasDelta =  (1.0f / m) * math::rowSum(lossd);

    return mWeights.transpose() * lossd;
}

math::MatrixF Layer::calculateGradients(const math::MatrixF& prevDz)
{
    const auto m = static_cast<float>(mInput.getColumnCount());

    const auto dz = prevDz.hadamardProduct(mDerivateFunc(mActivation));
    mWeightDelta = (1.0f / m) * (dz * mInput.transpose());
    mBiasDelta = (1.0f / m) * math::rowSum(dz);

    return mWeights.transpose() * dz;
}

void Layer::applyGradients(float learnSpeed)
{
    mWeights = mWeights - (learnSpeed * mWeightDelta);
    mBias = mBias - (learnSpeed * mBiasDelta);
}

}