#pragma once
#include <string>
#include <fstream>

class Pipe
{
	std::string pipeName;
public:
	Pipe(const std::string& name);
	~Pipe();

	std::string read();
	void write(const std::string& data);;
};
