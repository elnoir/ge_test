#include "ann.h"
#include <boost/range/algorithm/for_each.hpp>
#include <algorithm>

namespace ann {


float calculateLoss(const math::MatrixF &currentOutput, const math::MatrixF &expectedOutput)
{
    auto m = static_cast<float>(currentOutput.getColumnCount());
    auto L = -(1.0f/m) * (
        math::sum( currentOutput.apply(std::logf) * expectedOutput) ) +
        math::sum( (1.0f - currentOutput).apply(std::logf) * ( 1.0f - expectedOutput)
        );

    return L;
}


void ANN::configureNetwork(size_t inputSize)
{
    Layer::layerFx fLRU = [](math::MatrixF m) {
        std::function<float(float)> f = [](float value){ return value > 0.0f ? value : 0.0f; };
        return m.apply(f);
    };
    Layer::layerFx fLRUd = [](math::MatrixF m) {
        std::function<float(float)> f = [](float value){ return value > 0.0f ? 1.0f : 0.0f; };
        return m.apply(f);
    };
    mLayers.emplace_back(inputSize, 16, fLRU, fLRUd);
    mLayers.emplace_back(16, 16, fLRU, fLRUd);
    mLayers.emplace_back(16, 10, math::softMax<float>, fLRUd);
}


float ANN::train(const math::MatrixF &input, const math::MatrixF &expectedOutput)
{
    auto inputData = input;
    boost::for_each(mLayers, [&inputData](Layer &layer){
        inputData = layer.feedForward(std::move(inputData));
    });

    auto loss = calculateLoss(mLayers.back().getResult(), expectedOutput);

    auto d = mLayers.back().beginBackPropagation(expectedOutput);
    std::for_each(mLayers.rbegin() + 1, mLayers.rend(), [&d](Layer &layer){
        d = layer.calculateGradients(d);
    });

    boost::for_each(mLayers, [](Layer &layer){
        layer.applyGradients(1.0f);
    });

    return loss;
}


}