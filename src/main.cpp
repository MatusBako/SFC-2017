#include "layer.hpp"
#include "args.hpp"
#include "network.hpp"

void testParser(int argc, char** argv)
{
	Arguments args;
	if (!args.Parse(argc, argv))
		std::cout << "Parsing failed." << std::endl;

	args.PrintDebugValues();
}

int main(int argc, char** argv)
{
	/*
	InputLayer input = InputLayer(5);
	input.values[0] = 5;
	HiddenLayer hidden = HiddenLayer(input, 3);

	for (double val: input.getValues())
		std::cout << val << std::endl;*/

	//testParser(argc, argv);


    Arguments args;
    if (!args.Parse(argc, argv)) {
        std::cout << "Parsing failed." << std::endl;
    }
    else
    {
        Network net = Network(args);
        net.TrainWithOutput();
    }



	return 0;
}
