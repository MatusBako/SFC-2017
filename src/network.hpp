#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "args.hpp"
#include "layer.hpp"
#include "train_params.hpp"

#include <cmath>
#include <exception>
#include <string>
#include <vector>

class Network
{
public:
	void BGD(bool debugOutput);
	void SGD(bool debugOutput);
    void reloadArguments();

    explicit Network(Arguments args);
    ~Network();

private:
    void ForwardPass(bool debugOutput);
    void ComputeLastLayerDelta(int input_index, bool debugOutput);
    void ComputeDeltas(bool debugOutput);
    void ComputeWeights(bool debugOutput);
    void AdjustWeights(bool debugOutput);

    Arguments arguments;
	std::shared_ptr<TrainingParams> train_params = std::make_shared<TrainingParams>();
    std::shared_ptr<InputLayer> input_layer;
    std::vector<std::shared_ptr<HiddenLayer>> hidden_layers;
    double error_total;
    double error_expected;
    std::vector<std::vector<double>> input_data;
    std::vector<std::vector<double>> expected_output;
};

#endif
