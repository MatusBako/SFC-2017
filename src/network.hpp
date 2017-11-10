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
	explicit Network(Arguments args);

private:
    double Activation(double value);
    void ForwardPass();
    void ComputeLastLayerDelta(int input_index);
    void ComputeDeltas();
    void ComputeWeights();
    void AdjustWeights();

    std::vector<LayerAdapter> layers;
    double Error_total;
    double Error_expected;
    double lambda;
    double learning_rate;
    double momentum;
    std::vector<std::vector<double>> input_data;
    std::vector<std::vector<double>> expected;
};

#endif
