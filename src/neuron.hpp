#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>

// mi - learning rate
// alpha - momentum value
//delta_w = -mi * grad + alpha * delta_w_old

typedef struct t_neuron
{
	std::vector<double> weights;
	std::vector<double> delta_weights;
	double value;
	double sum;
	double delta;
} Neuron;

#endif
