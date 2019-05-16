#include "logic.h"
#include <vector>
#include <sstream>
#include <iostream>

const std::string OK = "OK";
const std::string FAILED = "FAILED";

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
				result = call(tokens[0], tokens[1]);
			else
				result = call(tokens[0], "");
		}
		else
			result = "FAILED";

		pipe.write(result);
	}
}

std::string Logic::call(const std::string& fn, const std::string& param)
{
	std::string result = FAILED;
	
	if (fn == "set-led-state")
	{
		bool newState;

		if (param == "on")
			newState = true;
		else if (param == "off")
			newState = false;
		else
			return FAILED + "\n";

		result = dev.setState(newState) ? OK : FAILED;
	}
	else if (fn == "get-led-state")
	{
		result = OK + " ";
		result += dev.getState() ? "on" : "off";
	}
	else if (fn == "set-led-color")
	{
		result = dev.setColor(param) ? OK : FAILED;
	}
	else if (fn == "get-led-color")
	{
		result = OK + " ";
		result += dev.getColor();
	}
	else if (fn == "set-led-rate")
	{
		int newRate = 0;
		try
		{
			newRate = std::stoi(param);
		}
		catch (...)
		{
			return FAILED + "\n";
		}

		result = dev.setRate(newRate) ? OK : FAILED;
	}
	else if (fn == "get-led-rate")
	{
		result = OK + " ";
		result += std::to_string(dev.getRate());
	}

	return result + "\n";
}
