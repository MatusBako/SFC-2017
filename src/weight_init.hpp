#include <random>

class WeightInitializer
{
public:
	static double initXavier(int input_count, int output_count)
	{
		double variance = 2/(input_count + output_count);

		std::default_random_engine generator;
		std::normal_distribution<double> distribution(0, variance);

		return distribution(generator);
	}

	static double initXavier(int input_count)
	{
		double variance = 1/input_count;

		std::default_random_engine generator;
		std::normal_distribution<double> distribution(0, variance);

		return distribution(generator);
	}
};