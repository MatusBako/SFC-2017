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

    unsigned long input_neuron_count = input_data[0].size();
    std::shared_ptr<InputLayer> input_layer = std::make_shared<InputLayer>(input_neuron_count);

    bool is_last_layer;

    for (int i = 0; i < arguments.layers.size(); i++)
    {
        auto neuron_count = arguments.layers[i];
        is_last_layer = (i == arguments.layers.size() - 1);

        if (!hidden_layers.empty())
            //create new Hidden layer with encapsulated previous layer
            hidden_layers.push_back(std::make_shared<HiddenLayer>(
                    std::make_shared<LayerAdapter>(hidden_layers[hidden_layers.size() - 1]), neuron_count, is_last_layer));
        else
            //create new Hidden layer with encapsulated input layer
            hidden_layers.push_back(std::make_shared<HiddenLayer>(
                    std::make_shared<LayerAdapter>(input_layer), neuron_count));
    }
}

Network::~Network()
{
    while (!hidden_layers.empty())
        hidden_layers.pop_back();
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
	for (auto &hidden_layer : hidden_layers)
        hidden_layer->computeValue(lambda);
}

void Network::ComputeLastLayerDelta(int input_index)
{
	//iterate over neurons of last layer (last layer doesn't have constant neuron)
	HiddenLayer& layer_last = (*hidden_layers[hidden_layers.size()-1]);
	double Error_sample = layer_last.computeLastLayerDelta(expected_output[input_index], lambda);
	error_total += Error_sample;
}

//not iterating over first two layers
void Network::ComputeDeltas()
{
	for (unsigned long layer_idx = hidden_layers.size() - 1; layer_idx > 0; layer_idx--)
		hidden_layers[layer_idx]->computePreviousLayerDelta(lambda);
}

void Network::ComputeWeights()
{
	for (auto &hidden_layer : hidden_layers)
        hidden_layer->computeWeights(learning_rate, momentum);
}

void Network::AdjustWeights()
{
	for (auto &hidden_layer : hidden_layers)
        hidden_layer->adjustWeights();
}
