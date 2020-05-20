#include "layer.h"

#include <random>
#include <cmath>
#include <boost/range/algorithm/fill.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace ann {

Layer::Layer(size_t inputRowCount, size_t outputRowCount, layerFx forwardFunc, layerFx derivateFunc)
    : mForwardFunc(std::move(forwardFunc))
    , mDerivateFunc(std::move(derivateFunc))
{
    mWeights = math::make_matrix<float>(outputRowCount, inputRowCount);
    mBias = math::make_matrix<float>(outputRowCount, 1);

    std::random_device rd{};
    std::mt19937 generator{rd()};
    std::normal_distribution<float> distribution{0.0f, 1.0f};

    const auto div = std::sqrtf(1.0f / inputRowCount);
    boost::for_each(mWeights, [&generator, &distribution, &div](float &value) {
        value = distribution(generator) / div;
    });
    boost::fill(mBias, 0.0f);
}

math::MatrixF Layer::feedForward(math::MatrixF &&inputData)
{
    mInput = std::move(inputData);
    mZ = (mWeights * mInput).addColumnVector(mBias);
    mActivation = mForwardFunc(mZ);
    return mActivation;
}

math::MatrixF Layer::beginBackPropagation(const math::MatrixF &expectedOutput)
{
    auto lossd = mActivation - expectedOutput;
    const auto m = mInput.getColumnCount();

    mWeightDelta = (1.0f / m) * mInput.transpose() * lossd;
    mBiasDelta =  (1.0f / m) * math::columnSum(lossd);
    return lossd * mWeights.transpose();
}

math::MatrixF Layer::calculateGradients(const math::MatrixF& prevDz)
{
    const auto m = mInput.getColumnCount();

    const auto dz = prevDz * mDerivateFunc(mActivation);
    mWeightDelta = (1.0f / m) * mInput.transpose() * dz;
    mBiasDelta = (1.0f / m) * math::columnSum(dz);

    return dz * mWeights.transpose();
}

void Layer::applyGradients(float learnSpeed)
{
    mWeights = mWeights - learnSpeed * mWeightDelta;
    mBias = mBias - learnSpeed * mBiasDelta;
}

}