#include "ann.h"
#include <boost/range/algorithm/for_each.hpp>
#include <algorithm>

namespace ann {


float calculateLoss(const math::MatrixF &currentOutput, const math::MatrixF &expectedOutput)
{
    auto m = static_cast<float>(expectedOutput.getRowCount());
    auto hatLog = currentOutput.apply(std::logf);
    auto yml = expectedOutput.hadamardProduct(hatLog);
    auto LSum = math::sum(yml);
    auto L = -(1.0f / m) * LSum;
    return L;
}

float sigmoid(float value)
{
    return 1 / (1 + std::expf(value));
}

float sigmoidDerived(float value)
{
    const auto s = sigmoid(value);
    return s * (1.0f - s);
}

float leakyLRU(float value)
{
    return std::max<float>(0.01f * value, value);
}

float leakyLRUDerived(float value)
{
    return (value < 0.0f) ? 0.01f : 1.0f;
}


void ANN::configureNetwork(size_t inputSize)
{
    Layer::layerFx f = [](const math::MatrixF &m) {
        return m.apply(leakyLRU);
    };

    Layer::layerFx fd = [](const math::MatrixF &m) {
        return m.apply(leakyLRUDerived);
    };

    const size_t layerSize1 = 128;
    const size_t layerSize2 = 64;

    mLayers.emplace_back(inputSize, layerSize1, f, fd);
    mLayers.emplace_back(layerSize1, layerSize2, f, fd);
    mLayers.emplace_back(layerSize2, 10, math::columnWiseSoftMax<float>, fd);
}


float ANN::train(const math::MatrixF &input, const math::MatrixF &expectedOutput)
{
    auto inputData = input;
    boost::for_each(mLayers, [&inputData](Layer &layer){
        inputData = layer.feedForward(inputData);
    });

    auto loss = calculateLoss(mLayers.back().getResult(), expectedOutput);

    auto d = mLayers.back().beginBackPropagation(expectedOutput);
    std::for_each(mLayers.rbegin() + 1, mLayers.rend(), [&d](Layer &layer){
        d = layer.calculateGradients(d);
    });

    boost::for_each(mLayers, [](Layer &layer){
        layer.applyGradients(0.5f);
    });

    return loss;
}

const math::MatrixF& ANN::test(const math::MatrixF &input)
{
    auto inputData = input;
    boost::for_each(mLayers, [&inputData](Layer &layer) {
        inputData = layer.feedForward(inputData);
    });
    return mLayers.back().getResult();
}

IANN::trainData ANN::collectTrainData()
{
    trainData result;

    auto activationResult = mLayers.back().getResult();
    const uint32_t numberOfSamples = static_cast<uint32_t>(activationResult.getColumnCount());
    result.push_back(numberOfSamples);
    for (size_t i = 0; i < numberOfSamples; ++i)
    {
        const uint32_t predictionLabel = static_cast<uint32_t>(math::getMaxElementIndex(activationResult.columnBegin(i), activationResult.columnEnd(i)));
        result.push_back(predictionLabel);
    }
    return result;
}

}