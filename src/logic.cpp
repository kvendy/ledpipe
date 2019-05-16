#include "logic.h"
#include <vector>
#include <sstream>
#include <iostream>

extern const std::string FAILED;
const std::string PARAMSFILENAME = "params.txt";

void trim(std::string& line)
{

	auto val = line.find_last_not_of(" \n\r\t") + 1;

	if (val == line.size() || val == std::string::npos) {
		val = line.find_first_not_of(" \n\r\t");
		line = line.substr(val);
	}
	else
		line.erase(val);
}

Logic::Logic(const std::string& pipeName) :
	pipe(pipeName)
{
}


Logic::~Logic()
{
}

void Logic::run()
{
	dev.start();
	
	for (;;)
	{
		std::string input = pipe.read(), result, token;
		std::stringstream ss(input);
		std::vector<std::string> tokens;
		while (std::getline(ss, token, ' '))
		{
			trim(token);
			tokens.push_back(token);
		}

		if (!tokens.empty())
		{
			if (tokens.size() > 1)
				result = dev.call(tokens[0], tokens[1]);
			else
				result = dev.call(tokens[0], "");
		}
		else
			result = FAILED;

		pipe.write(result);
	}
}

void Logic::createMethodsFile()
{
	std::ofstream outfile;
	outfile.open(PARAMSFILENAME);

	if (outfile.is_open())
	{
		std::map<std::string, std::string> descriptions = dev.listMethods();
			for (auto it : descriptions)
				outfile << it.first << "|" << it.second << std::endl;
	}
}
