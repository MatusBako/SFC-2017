#include <random>

class WeightInitializer
{
public:
    explicit WeightInitializer() {}
    WeightInitializer(const WeightInitializer&) = delete;
    WeightInitializer& operator=(const WeightInitializer&) = delete;

    double initXavier(int input_count, int output_count)
	{
		double variance = 2/(input_count + output_count);

		std::normal_distribution<double> distribution(0, variance);

		return distribution(generator);
	}

	double initXavier(int input_count)
	{
		double variance = 1./input_count;

		std::normal_distribution<double> distribution(0, variance);

		return distribution(generator);
	}

private:
    std::default_random_engine generator;
};

