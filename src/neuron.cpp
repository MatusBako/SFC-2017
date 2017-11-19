#include "neuron.hpp"

Neuron::Neuron(std::shared_ptr<WeightInitializer> initalizer,
			   std::shared_ptr<TrainingParams> train_params,
			   int weight_count):
delta_weights(std::vector<double >(weight_count)),
train_params(train_params)
{
    for (int weight_idx = 0; weight_idx < weight_count; weight_idx++)
        weights.push_back(initalizer->initXavier(weight_count));
}

Neuron::Neuron(std::shared_ptr<TrainingParams> train_params,double value):
	train_params(train_params),
	value(value)
{
}

void Neuron::computeValue(std::shared_ptr<std::vector<double>> inputs)
{
    sum = 0;

    for (int i = 0; i < weights.size(); i++)
        sum += weights[i] * (*inputs)[i];

    value = 1/(1 + exp(-train_params->lambda * sum));
}

double Neuron::getValue()
{
    return value;
}

double Neuron::computeLastLayerDelta(double expected)
{
    double error = 0.5 * (expected - value) * (expected - value);
    delta = (expected - value) * train_params->lambda * value * (1 - value);

    return error;
}

double Neuron::getWeightedDelta(uint weight_index)
{
    return delta * weights[weight_index];
}

void Neuron::computeDelta(double deltas_summed)
{
    delta = deltas_summed * train_params->lambda * value * (1 - value);
}

void Neuron::computeWeights(std::vector<double>& inputs)
{
    //iterating over all weights (also first neuron)
    for (int weight_idx = 0; weight_idx < inputs.size(); weight_idx++)
    {
        delta_weights[weight_idx] = -train_params->learning_rate * delta
                                    * inputs[weight_idx] + train_params->momentum * delta_weights[weight_idx];
    }
}

void Neuron::adjustWeights()
{
    for (int weight_idx = 0; weight_idx < weights.size(); weight_idx++)
        weights[weight_idx] += delta_weights[weight_idx];
}
