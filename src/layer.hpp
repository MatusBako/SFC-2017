#ifndef LAYER_HPP
#define LAYER_HPP

#include "neuron.hpp"
#include "weight_init.hpp"

#include <functional>
#include <memory>
#include <vector>

class LayerAdapter;

class Layer
{
public:	
	int node_count;

protected:
	Layer() {};
};

class InputLayer: public Layer
{
public:	
	InputLayer(int input_count);

	std::vector<double> getValues();
	void setValues(std::vector<double>* v);

//private:
	std::vector<double> values;
};


class HiddenLayer: public Layer
{
public:
	HiddenLayer(std::shared_ptr<LayerAdapter> input, int neuron_count);

	std::vector<double> getValues();
	void computeValue(double lambda);
	void computePreviousLayerDelta(double lambda);
	double computeLastLayerDelta(const std::vector<double>& expected, double lambda);
	void computeWeights(double learning_rate, double momentum);
	void adjustWeights();

private:
	std::shared_ptr<LayerAdapter> previous_layer;
	std::vector<Neuron> neurons;
};

class LayerAdapter
{
public:
	LayerAdapter(std::shared_ptr<HiddenLayer> layer);
	LayerAdapter(std::shared_ptr<InputLayer> layer);
	
	std::vector<double> getValues();
    int getNodeCount();

//private:
	std::shared_ptr<HiddenLayer> hidden = nullptr;
	std::shared_ptr<InputLayer> input = nullptr;
};

#endif
