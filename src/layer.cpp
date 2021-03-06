#include <iostream>
#include <utility>
#include "layer.hpp"

Layer::Layer(int node_count):
    node_count(node_count)
{
}

int Layer::getNodeCount()
{
    return node_count;
}

InputLayer::InputLayer(int input_count):
    Layer(input_count + 1)
{
	values = std::vector<double>(input_count + 1, 0);
}

std::vector<double> InputLayer::getValues()
{
	return values;
}

void InputLayer::setValues(const std::vector<double>& src, bool debugOutput)
{
    if (src.size() != node_count - 1)
        throw ("Number of inputs does not equal node count - 1.");

	if (debugOutput)
	{
		std::cout << "Inputs: ";
		for (int i = 0; i < src.size(); i++)
		{
			values[i+1] = src[i];
			std::cout << src[i] << ", ";
		}
		std::cout << std::endl;
	}
	else
		for (int i = 0; i < src.size(); i++)
			values[i+1] = src[i];
}

HiddenLayer::HiddenLayer(std::shared_ptr<WeightInitializer> initalizer, std::shared_ptr<TrainingParams> train_params,
						 std::shared_ptr<LayerAdapter> input_layer,int neuron_count, bool is_last):
    Layer(neuron_count),
    is_last_layer(is_last),
    previous_layer(input_layer)
{
    if (!is_last)
    {
        node_count++;
        neurons.emplace_back(train_params,1);
    }

    for (int i = 0; i < neuron_count; i++)
		neurons.emplace_back(initalizer, train_params, previous_layer->getNodeCount());
}

std::vector<double> HiddenLayer::getValues()
{
	std::vector<double> values;

	for (Neuron neuron: neurons)
		values.push_back(neuron.getValue());
	return values;
}

void HiddenLayer::computeValue()
{
	auto inputs = previous_layer->getValues();
	auto p_inputs = std::make_shared<std::vector<double>>(inputs);

	for (int neuron_idx = is_last_layer ? 0 : 1; neuron_idx < neurons.size(); neuron_idx++)
		neurons[neuron_idx].computeValue(p_inputs);
}

double HiddenLayer::computeLastLayerDelta(const std::vector<double>& expected)
{
	double error = 0;
	for (int neuron_idx = 0; neuron_idx < neurons.size(); neuron_idx++)
	{
		Neuron& neuron = neurons[neuron_idx];
		error += neuron.computeLastLayerDelta(expected[neuron_idx]);
	}
	return error;
}

void HiddenLayer::computePreviousLayerDelta()
{
	//TODO: je to vzdy hidden layer? => ANO
	// pocicat deltu pre vstupny neuron nema zmysel
	// musia sa menit aj vahy smerujuce ku vstupnemu vektoru
	HiddenLayer& layer_prev = (*previous_layer->hidden);

	// first neuron is constant but his weight also needs to be changed
	for (int neuron_prev_idx = 0; neuron_prev_idx < layer_prev.neurons.size(); neuron_prev_idx++)
	{
		Neuron& neuron_prev = layer_prev.neurons[neuron_prev_idx];

		double sum = 0;

		for (uint neuron_idx = (is_last_layer? 0 : 1); neuron_idx < neurons.size(); neuron_idx++)
			sum += neurons[neuron_idx].getWeightedDelta(neuron_prev_idx);

		neuron_prev.computeDelta(sum);
	}
}

void HiddenLayer::computeWeights()
{
	std::vector<double> inputs = previous_layer->getValues();

	for (int neuron_idx = (is_last_layer ? 0 : 1); neuron_idx < neurons.size(); neuron_idx++)
		neurons[neuron_idx].computeWeights(inputs);
}

void HiddenLayer::adjustWeights()
{
    for (int neuron_idx = (is_last_layer ? 0 : 1); neuron_idx < neurons.size(); neuron_idx++)
		neurons[neuron_idx].adjustWeights();
}

void HiddenLayer::printValues(std::string start, std::string delim)
{
	for (auto neuron: neurons)
		std::cout << neuron.getValue() << delim;
	std::cout << std::endl;
}

void HiddenLayer::printDeltas(std::string start, std::string delim)
{
	std::cout << start;
	for (auto neuron: neurons)
		std::cout << neuron.getDelta() << delim;
	std::cout << std::endl;
}

void HiddenLayer::printWeightDeltas(std::string start, std::string delim)
{
	for (int neuron_idx = (is_last_layer ? 0 : 1); neuron_idx < neurons.size(); neuron_idx++)
	{
		std::cout << start << "N" << neuron_idx << ": ";
		for (auto weight: neurons[neuron_idx].getWeightDeltas())
			std::cout << weight << ", ";
		std::cout << std::endl;
	}
}

void HiddenLayer::printWeights(std::string start, std::string delim)
{
	for (int neuron_idx = (is_last_layer ? 0 : 1); neuron_idx < neurons.size(); neuron_idx++)
	{
		std::cout << start << "N" << neuron_idx << ": ";
		for (auto weight: neurons[neuron_idx].getWeights())
			std::cout << weight << ", ";
		std::cout << std::endl;
	}
}


LayerAdapter::LayerAdapter(std::shared_ptr<HiddenLayer> layer)
{
    hidden = layer;
    input = nullptr;
}

LayerAdapter::LayerAdapter(std::shared_ptr<InputLayer> layer)
{
	hidden = nullptr;
    input = layer;
}

std::vector<double> LayerAdapter::getValues()
{
    if (hidden)
        return hidden->getValues();
    return input->getValues();
}

int LayerAdapter::getNodeCount()
{
    if (hidden)
        return hidden->getNodeCount();
    return input->getNodeCount();
}
