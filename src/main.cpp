#include "layer.hpp"
#include "args.hpp"
#include <iostream>

void testParser(int argc, char** argv)
{
	Arguments args;
	if (!args.Parse(argc, argv))
		std::cout << "Parsing failed." << std::endl;

	if (args.argument_set['l'])
	{
		std::cout << "Layers : " << std::endl;
		for (int i = 0; i < args.layers.size(); i++)
			std:: cout << args.layers[i] << " ";
		std:: cout << std::endl;
	}

	if (args.argument_set['r'])
		std::cout << "lr - " << args.learning_rate << std::endl;

	if (args.argument_set['b'])
		std::cout << "lambda " << args.lambda << std::endl;

	if (args.argument_set['m'])
		std::cout << "momentum " << args.momentum << std::endl;

	if (args.argument_set['f'])
	{
		std::cout << "Input data:" << std::endl;
		for (auto v: args.input_data)
		{
			for (auto d: v)
				std::cout << "'" << d << "' ";
			std::cout << std::endl;
		}

		std::cout << "Expected data:" << std::endl;
		for (auto v: args.expected_output)
		{
			for (auto d: v)
				std::cout << "'" << d << "' ";
			std::cout << std::endl;
		}

	}
}

int main(int argc, char** argv)
{
	/*
	InputLayer input = InputLayer(5);
	input.values[0] = 5;
	HiddenLayer hidden = HiddenLayer(input, 3);

	for (double val: input.getValues())
		std::cout << val << std::endl;*/

	testParser(argc, argv);
	return 0;
}
