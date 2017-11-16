#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "args.hpp"
#include "layer.hpp"

#include <cmath>
#include <vector>

class Network
{
public:
	void Train();
	double computeGradient();
	void updateWeights();
    void reloadArguments();

    explicit Network(Arguments args);
    ~Network();

private:
    void ForwardPass();
    void ComputeLastLayerDelta(int input_index);
    void ComputeDeltas();
    void ComputeWeights();
    void AdjustWeights();


    Arguments arguments;
    std::shared_ptr<InputLayer> input_layer;
    std::vector<std::shared_ptr<HiddenLayer>> hidden_layers;
    double error_total;
    double error_expected;
    double lambda;
    double learning_rate;
    double momentum;
    std::vector<std::vector<double>> input_data;
    std::vector<std::vector<double>> expected_output;
};

#endif
