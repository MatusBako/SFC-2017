#ifndef TRAIN_PARAMS_HPP
#define TRAIN_PARAMS_HPP

struct TrainingParams
{
	TrainingParams() = default;
	TrainingParams(double lambda, double learning_rate, double momentum):
		lambda(lambda),
		learning_rate(learning_rate),
		momentum(momentum)
	{}

	double lambda;
	double learning_rate;
	double momentum;
};

#endif //SFC2017_TRAIN_PARAMS_HPP
