#include "network.hpp"

Network::Network(Arguments args)
{
    arguments = args;
    reloadArguments();
}


void Network::reloadArguments()
{
    lambda = arguments.lambda;
    learning_rate = arguments.learning_rate;
    momentum = arguments.momentum;
    error_expected = arguments.expected_error;
    input_data = arguments.input_data;
    expected_output = arguments.expected_output;

    int input_neuron_count = input_data[0].size();
    std::shared_ptr<InputLayer> input_layer = std::make_shared<InputLayer>(input_neuron_count);

    for (unsigned int neuron_count: arguments.layers)
    {
        if (hidden_layers.size())
            //create new Hidden layer with encapsulated previous layer
            hidden_layers.push_back(std::make_shared<HiddenLayer>(
                    std::make_shared<LayerAdapter>(hidden_layers[hidden_layers.size() - 1]), neuron_count));
        else
            //create new Hidden layer with encapsulated input layer
            hidden_layers.push_back(std::make_shared<HiddenLayer>(
                    std::make_shared<LayerAdapter>(input_layer), neuron_count));
    }
}

Network::~Network()
{
    while (hidden_layers.size())
        hidden_layers.pop_back();
}

double Network::Activation(double value)
{
	return 1/(1+exp(-lambda*value));
}

void Network::Train()
{
	do 
	{ 
		error_total = 0;
		for (int input_index = 0; input_index < input_data.size(); input_index++)
		{
			input_layer->setValues(&input_data[input_index]);

			ForwardPass();

			ComputeLastLayerDelta(input_index);

			ComputeDeltas();

			ComputeWeights();
			AdjustWeights();
 		}
 	}
 	while (error_total > error_expected);

}

void Network::ForwardPass() 
{
	//first layer only contains inputs
	for (int layer_idx = 0; layer_idx < hidden_layers.size(); layer_idx++)
		hidden_layers[layer_idx]->computeValue();
}

void Network::ComputeLastLayerDelta(int input_index)
{
	//iterate over neurons of last layer (last layer doesn't have constant neuron)
	HiddenLayer& layer_last = (*hidden_layers[hidden_layers.size()-1]);
	double Error_sample = layer_last.computeLastLayerDelta(expected_output[input_index]);
	error_total += Error_sample;
}

//not iterating over first two layers
void Network::ComputeDeltas()
{
	for (int layer_idx = hidden_layers.size() - 1; layer_idx > 0; layer_idx--)
		hidden_layers[layer_idx]->computePreviousLayerDelta();
}

void Network::ComputeWeights()
{
	for (int layer_idx = 0; layer_idx < hidden_layers.size(); layer_idx++)
		hidden_layers[layer_idx]->computeWeights();
}

void Network::AdjustWeights()
{
	for (int layer_idx = 0; layer_idx < hidden_layers.size(); layer_idx++)
		hidden_layers[layer_idx]->adjustWeights();
}
