#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>
#include <memory>
#include "weight_init.hpp"
#include "train_params.hpp"

// mi - learning rate
// alpha - momentum value
//delta_w = -mi * grad + alpha * delta_w_old

class Neuron
{
public:
    Neuron(std::shared_ptr<WeightInitializer> initalizer,
		   std::shared_ptr<TrainingParams> train_params,
		   int weight_count);
    Neuron(std::shared_ptr<TrainingParams> train_params,double value);

	double getValue();
	double getDelta();
	const std::vector<double>& getWeights();
	const std::vector<double>& getWeightDeltas();

	double getWeightedDelta(uint weight_index);
    void computeValue(std::shared_ptr<std::vector<double>> inputs);
	double computeLastLayerDelta(double expected);
	void computeDelta(double deltas_summed);
	void computeWeights(std::vector<double>& inputs);
	void adjustWeights();

private:
	std::shared_ptr<TrainingParams> train_params;
	std::vector<double> weights;
	std::vector<double> delta_weights;
	double value;
	double sum;
	double delta;
};

#endif
