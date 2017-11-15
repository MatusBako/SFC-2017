#ifndef ARGS_HPP
#define ARGS_HPP

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Arguments
{
public:
	bool Parse(int argc, char** argv);

//private:
	std::vector<unsigned int> layers;
	double learning_rate;
	double lambda;
	double momentum;
    double expected_error;
	std::map<char, bool> argument_set;
	//TODO training and testing data
	std::vector<std::vector<double>> input_data;
	std::vector<std::vector<double>> expected_output;

	bool PrintErrAndReturn(std::string message);
	void Split(const std::string &s, char delim, std::vector<std::string>& result);	
	void SplitWhitespaces(const std::string& input, std::vector<std::string>& result);
};

#endif