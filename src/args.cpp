#include "args.hpp"

bool Arguments::Parse(int argc, char** argv)
{
    if (argc == 1)
    {
        Arguments::PrintHelp();
        return 1;
    }


	std::string::size_type size;

	for (int i = 1; i < argc;)
	{
		// layers
		if (!strcmp(argv[i], "-l") || !strcmp(argv[i],"--layers"))
		{
			if (argument_set['l'])
				return PrintErrAndReturn("Layers option entered more than once.");

			if (i+1 >= argc)
				return PrintErrAndReturn("Value for layers parameter not entered.");

			std::string s = static_cast<std::string>(argv[++i]);
			std::vector<std::string> numbers;
			Arguments::Split(s, ',',numbers);

			for (std::string number: numbers)
			{
				int converted = std::stoi(number, &size);
				// TODO: assert

				if (number.length() != size)
					return PrintErrAndReturn("Conversion error while processing layers.");

				else if (converted < 1)
					return PrintErrAndReturn("Number of neurons in layer must be bigger than 0.");

				layers.push_back(static_cast<uint>(converted));
			}
			argument_set['l'] = true;
		}

		// learning rate
		else if (!strcmp(argv[i], "-r") || !strcmp(argv[i],"--learning-rate"))
		{
			if (argument_set['r'])
				return PrintErrAndReturn("Learning rate option entered more than once.");

			if (i+1 >= argc)
				return PrintErrAndReturn("Value for learning rate not entered.");

			std::string s = static_cast<std::string>(argv[++i]);
			double converted = std::stod(s, &size);

			if (s.length() != size)
				return PrintErrAndReturn("Conversion error (learning rate).");

			if (converted <= 0 || converted > 1)
				return PrintErrAndReturn("Learning rate not in interval (0, 1>.");

			learning_rate = converted;
			argument_set['r'] = true;
		}

		// lambda
		else if (!strcmp(argv[i], "-b") || !strcmp(argv[i],"--lambda"))
		{
			if (argument_set['b'])
				return PrintErrAndReturn("Lambda option entered more than once.");

			if (i+1 >= argc)
				return PrintErrAndReturn("Value for lambda not entered.");

			std::string s = static_cast<std::string>(argv[++i]);
			double converted = std::stod(s, &size);

			if (s.length() != size)
				return PrintErrAndReturn("Conversion error (lambda).");

			lambda = converted;
			argument_set['b'] = true;
		}

			// momentum
		else if (!strcmp(argv[i], "-m") || !strcmp(argv[i],"--momentum"))
		{
			if (argument_set['m'])
				return PrintErrAndReturn("Momentum option entered more than once.");

			if (i+1 >= argc)
				return PrintErrAndReturn("Value for momentum not entered.");

			std::string s = static_cast<std::string>(argv[++i]);
			double converted = std::stod(s, &size);

			if (s.length() != size)
				return PrintErrAndReturn("Conversion error (momentum).");


			if (converted < 0.5 || converted > 0.95)
				return PrintErrAndReturn("Momentum not in interval <0.5, 0.95>.");

			momentum = converted;
			argument_set['m'] = true;
		}

		// error
		else if (!strcmp(argv[i], "-e") || !strcmp(argv[i],"--error"))
		{
			if (argument_set['e'])
				return PrintErrAndReturn("Error option entered more than once.");

			if (i+1 >= argc)
				return PrintErrAndReturn("Value for expected error not entered.");

			std::string s = static_cast<std::string>(argv[++i]);
			double converted = std::stod(s, &size);

			if (s.length() != size)
				return PrintErrAndReturn("Conversion error (error).");

			if (converted < 0)
				//TODO: proper error interval
				return PrintErrAndReturn("Expected error not in interval (0, ?>.");

			expected_error = converted;
			argument_set['e'] = true;
		}

		// input file
		else if (!strcmp(argv[i], "-f") || !strcmp(argv[i],"--input-file"))
		{
			if (argument_set['f'])
				return PrintErrAndReturn("File option entered more than once.");
			argument_set['f'] = true;


			if (i+1 >= argc)
				return PrintErrAndReturn("Value for input file not entered.");

			std::ifstream input_file;
  			input_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				input_file.open(argv[++i]);
			}
			catch (const std::ifstream::failure& e)
			{
				return PrintErrAndReturn("Error occured while opening input file.");
			}

			std::string line;
			std::string::size_type size;
			std::vector<std::string> parsed;
			std::vector<int> converted;

			// load first line
			getline(input_file, line);

			// parse first line 
			SplitWhitespaces(line, parsed);

			if (parsed.size() != 3)
				return PrintErrAndReturn("size of parsed - " + std::to_string(parsed.size()) + "\nFirst line must contain number of lines, input count per line and expected count per line.");

			for (std::string s: parsed)
			{
				int result = std::stoi(s, &size);

                if (s.length() != size)
					return PrintErrAndReturn("Conversion to number failed on the first line.");

				if (result < 1)
					return PrintErrAndReturn("Numbers on first line must be bigger than 0.");

				converted.push_back(result);
			}

			int line_count = converted[0];
			int input_count = converted[1];
			int expected_count = converted[2];

			//parse lines
			for (int line_index = 0; line_index < line_count; line_index++)
			{
				getline(input_file, line);

				parsed.clear();
				SplitWhitespaces(line, parsed);

				if (parsed.size() != input_count + expected_count)
					return PrintErrAndReturn("Wrong number count on line " + std::to_string(line_index+1) + ".");

                input_data.emplace_back();
                expected_output.emplace_back();

				for (int input_index = 0; input_index < input_count; input_index++)
				{
					double result = std::stod(parsed[input_index], &size);

					if (parsed[input_index].length() != size)
						return PrintErrAndReturn("Conversion to number failed on " + std::to_string(line_index+1) + ". line.");

					input_data[line_index].push_back(result);
				}

				for (int expected_index = 0; expected_index < expected_count; expected_index++)
				{
					double result = std::stod(parsed[input_count + expected_index], &size);
					if (parsed[expected_index].length() != size)
					{
						std::string message = "Conversion to number failed on "+ std::to_string(line_index+1) + ". line.";
						return PrintErrAndReturn(message);
					}

					expected_output[line_index].push_back(result);
				}
			}

			try
			{
				input_file.close();
			}
			catch (const std::ifstream::failure& e)
			{
				return PrintErrAndReturn("Error occured while opening input file.");
			}

		}

		else
			return PrintErrAndReturn("Wrong parameter.");

		i++;
	}
	
	std::cout << "argument count - " << argument_set.size() << std::endl;

    // TODO: for debugging purposes
	return argument_set.size() >= 6;

}

bool Arguments::PrintErrAndReturn(std::string message)
{
	std::cerr << message << std::endl;
	return false;
}

void Arguments::Split(const std::string &s, char delim, std::vector<std::string>& result)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
    {
        result.push_back(item);
    }
}

void Arguments::SplitWhitespaces(const std::string& input, std::vector<std::string>& result)
{
	std::istringstream iss(input);
	std::vector<std::string> tokens;
	copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(result)
	);
}

void Arguments::PrintDebugValues()
{
    if (argument_set['l'])
    {
        std::cout << "Layers : " << std::endl;
        for (int i = 0; i < layers.size(); i++)
            std:: cout << layers[i] << " ";
        std:: cout << std::endl;
    }

	if (argument_set['r'])
		std::cout << "lr - " << learning_rate << std::endl;

	if (argument_set['e'])
		std::cout << "e - " << expected_error << std::endl;

    if (argument_set['b'])
        std::cout << "lambda " << lambda << std::endl;

    if (argument_set['m'])
        std::cout << "momentum " << momentum << std::endl;

    if (argument_set['f'])
    {
        std::cout << "Input data:" << std::endl;
        for (auto v: input_data)
        {
            for (auto d: v)
                std::cout << "'" << d << "' ";
            std::cout << std::endl;
        }

        std::cout << "Expected data:" << std::endl;
        for (auto v: expected_output)
        {
            for (auto d: v)
                std::cout << "'" << d << "' ";
            std::cout << std::endl;
        }

    }
}

void Arguments::PrintHelp()
{
    std::cout << "Required arguments: " << std::endl
            << "-m, --momentum: momentum" << std::endl
            << "-r, --learning-rate: momentum" << std::endl
            << "-l, --layers: numbers of neuron in layers with delimeter \',\'" << std::endl
            << "-b, --lambda: momentum" << std::endl
            << "-f, --input-file" <<    std::endl
            << "-e, --error" <<    std::endl;
}

Arguments::Arguments(const Arguments &obj):
	argument_set(obj.argument_set),
	expected_error(obj.expected_error),
	lambda(obj.lambda),
	learning_rate(obj.learning_rate),
	momentum(obj.momentum),
	layers(obj.layers),
	input_data(obj.input_data),
	expected_output(obj.expected_output)
{

}

Arguments::Arguments()
{
}
