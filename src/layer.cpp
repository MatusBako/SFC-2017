#include "layer.hpp"

InputLayer::InputLayer(int input_count)
{
	values = std::vector<double>(input_count + 1, 0);
	node_count = input_count + 1;
}

std::vector<double> InputLayer::getValues()
{
	return values;
}

void InputLayer::setValues(std::vector<double>* src)
{
    if (src->size() != node_count - 1)
        throw ("Number of inputs does not equal node count - 1.");

	for (int i = 0; i < src->size(); i++)
		values[i+1] = (*src)[i];
}

HiddenLayer::HiddenLayer(std::shared_ptr<LayerAdapter> input_layer,int neuron_count)
{

    node_count = neuron_count + 1;

    Neuron neuron = {.value = 1};
    neurons.push_back(neuron);

    for (int i = 1; i < neuron_count + 1; i++)
	{
        neuron = Neuron();

		for (int weight_idx = 0; weight_idx < previous_layer->getNodeCount(); weight_idx++)
			neuron.weights.push_back(WeightInitializer::initXavier(previous_layer->getNodeCount()));

		neurons.push_back(neuron);
	}

}

std::vector<double> HiddenLayer::getValues()
{
	std::vector<double> values;

	for (Neuron neuron: neurons)
		values.push_back(neuron.value);
	return values;
}

void HiddenLayer::computeValue(double lambda)
{
	for (int neuron_idx = 1; neuron_idx < this->neurons.size(); neuron_idx++)
	{
		Neuron& neuron = this->neurons[neuron_idx];
		neuron.value = neuron.sum = 0;

		//TODO: more efficient way of getting inputs?
		std::vector<double> inputs = this->previous_layer->getValues();

		for (int i = 0; i < neuron.weights.size(); i++)
			neuron.sum += neuron.weights[i] * inputs[i];

        //TODO: activation
        neuron.value = 1/(1 + exp(-lambda * neuron.sum));
	}
}

double HiddenLayer::computeLastLayerDelta(const std::vector<double>& expected, double lambda)
{
	double error = 0;
	for (int neuron_idx = 0; neuron_idx < this->neurons.size(); neuron_idx++)
	{
		Neuron& neuron = this->neurons[neuron_idx];

		error += 0.5 * (expected[neuron_idx] - neuron.value) * (expected[neuron_idx] - neuron.value);
		neuron.delta = (expected[neuron_idx] - neuron.value) * lambda * neuron.value * (1 - neuron.value);
	}
	return error;
}

void HiddenLayer::computePreviousLayerDelta(double lambda)
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

		for (int neuron_idx = 1; neuron_idx < this->neurons.size(); neuron_idx++)
		{
			Neuron& neuron = this->neurons[neuron_idx];
			sum += neuron.delta * neuron.weights[neuron_prev_idx];
		}

		neuron_prev.delta = sum * lambda * neuron_prev.value * (1 - neuron_prev.value);
	}
}

void HiddenLayer::computeWeights(double learning_rate, double momentum)
{				
	//not iterating over first neuron
	for (int neuron_idx = 1; neuron_idx < neurons.size(); neuron_idx++)
	{
		Neuron& neuron = neurons[neuron_idx];
		
		std::vector<double> neuron_inputs(1,1);
		auto values = previous_layer->getValues();
		neuron_inputs.insert(neuron_inputs.end(), values.begin(), values.end());

		//iterating over all weights (also first neuron)
		for (int weight_idx = 0; weight_idx < neuron.weights.size(); weight_idx++)
		{
			neuron.delta_weights[weight_idx] = -learning_rate * neuron.delta * neuron_inputs[weight_idx] 
				+ momentum * neuron.delta_weights[weight_idx];
			neuron.weights[weight_idx] += neuron.delta_weights[weight_idx];
		}
	}
}

void HiddenLayer::adjustWeights()
{
    for (int neuron_idx = 1; neuron_idx < neurons.size(); neuron_idx++)
	{
		Neuron& neuron = neurons[neuron_idx];
		
		//iterating over all weights (also first neuron)
		for (int weight_idx = 0; weight_idx < neuron.weights.size(); weight_idx++)
			neuron.weights[weight_idx] += neuron.delta_weights[weight_idx];
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
        return hidden->node_count;
    return input->node_count;
}
