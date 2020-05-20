#include <ann/iann.h>
#include "layer.h"


namespace ann {

class ANN:
    public IANN
{
private:
    std::vector<Layer> mLayers;
public:
    void configureNetwork(size_t inputSize) override;
    float train(const math::MatrixF &inputs, const math::MatrixF &expectedOutput) override;
};

}