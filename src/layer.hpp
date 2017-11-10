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
	//virtual void passValues(std::vector<std::reference_wrapper<double>>& dest) = 0;

protected:
	Layer() {};
};

class InputLayer: public Layer
{
public:	
	InputLayer(int input_count);

	std::vector<double> getValues();
	//void passValues(std::vector<std::reference_wrapper<double>>& dest);
	void setValues(std::vector<double> v);

//private:
	std::vector<double> values;
};


class HiddenLayer: public Layer
{
public:
	HiddenLayer(Layer& input, int neuron_count);

	std::vector<double> getValues();
	//void passValues(std::vector<std::reference_wrapper<double>>& dest);
	void computeValue();
	void computePreviousLayerDelta();
	double computeLastLayerDelta(const std::vector<double>& expected);
	void computeWeights();
	void adjustWeights();

//private:
	std::shared_ptr<LayerAdapter> previous_layer;
	std::vector<Neuron> neurons;
	double lambda;
	double momentum;
	double learning_rate;
	//std::vector<std::reference_wrapper<double>> input_values;
};

class LayerAdapter
{
public:
	LayerAdapter(std::shared_ptr<HiddenLayer> layer);
	LayerAdapter(std::shared_ptr<InputLayer> layer);
	
	std::vector<double> getValues();
	void computeValue();

//private:
	std::shared_ptr<HiddenLayer> hidden = nullptr;
	std::shared_ptr<InputLayer> input = nullptr;
};

#endif
