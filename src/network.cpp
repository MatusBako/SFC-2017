#include "network.hpp"

Network::Network(Arguments args)
{
	lambda = args.lambda;
	learning_rate = args.learning_rate;
	momentum = args.momentum;

	Error_expected = args.
}

double Network::Activation(double value)
{
	return 1/(1+exp(-lambda*value));
}

void Network::Train()
{
	do 
	{ 
		Error_total = 0;
		for (int input_index = 0; input_index < input_data.size(); input_index++)
		{
			// load inputs into net
			layers[0].input->setValues(input_data[input_index]);

			ForwardPass();

			ComputeLastLayerDelta(input_index);

			ComputeDeltas();

			ComputeWeights();
			AdjustWeights();
 		}
 	}
 	while (Error_total > Error_expected);

}

void Network::ForwardPass() 
{
	//first layer only contains inputs
	for (int layer_idx = 1; layer_idx < layers.size(); layer_idx++)
		layers[layer_idx].hidden->computeValue();
}

void Network::ComputeLastLayerDelta(int input_index)
{
	//iterate over neurons of last layer (last layer doesn't have constant neuron)
	HiddenLayer& layer_last = (*layers[layers.size()-1].hidden);
	double Error_sample = layer_last.computeLastLayerDelta(expected[input_index]);
	Error_total += Error_sample;
}

//not iterating over first two layers
void Network::ComputeDeltas()
{
	for (int layer_idx = layers.size() - 1; layer_idx > 1; layer_idx--)
		layers[layer_idx].hidden->computePreviousLayerDelta();
}

void Network::ComputeWeights()
{
	for (int layer_idx = 1; layer_idx < layers.size(); layer_idx++)
		layers[layer_idx].hidden->computeWeights();
}

void Network::AdjustWeights()
{
	for (int layer_idx = 1; layer_idx < layers.size(); layer_idx++)
		layers[layer_idx].hidden->adjustWeights();
}
