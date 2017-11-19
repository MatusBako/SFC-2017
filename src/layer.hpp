#ifndef LAYER_HPP
#define LAYER_HPP

#include "neuron.hpp"
#include "weight_init.hpp"
#include "train_params.hpp"

#include <functional>
#include <memory>
#include <vector>

class LayerAdapter;

class Layer
{
public:
    int getNodeCount();
	int node_count;

protected:
	Layer(int node_count);
};

class InputLayer: public Layer
{
public:	
	InputLayer(int input_count);

	std::vector<double> getValues();
	void setValues(const std::vector<double>& v);

private:
	std::vector<double> values;
};


class HiddenLayer: public Layer
{
public:
	HiddenLayer(std::shared_ptr<WeightInitializer> initalizer, std::shared_ptr<TrainingParams> train_params,  std::shared_ptr<LayerAdapter> input, int neuron_count, bool is_last);

	std::vector<double> getValues();
	void computeValue();
	void computePreviousLayerDelta();
	double computeLastLayerDelta(const std::vector<double>& expected);
	void computeWeights();
	void adjustWeights();

	void printValues(std::string start, std::string delim);
	void printDeltas(std::string start, std::string delim);
	void printWeightDeltas(std::string start, std::string delim);
	void printWeights(std::string start, std::string delim);

private:
	std::shared_ptr<LayerAdapter> previous_layer;
	std::vector<Neuron> neurons;
    bool is_last_layer;
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
