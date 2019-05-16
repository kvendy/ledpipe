#pragma once
#include <string>
#include <fstream>

#include <Windows.h>

class Pipe
{
	std::string pipeName;
	HANDLE hPipe;
public:
	Pipe(const std::string& name);
	~Pipe();

	std::string read();
	void write(const std::string& data);;
};
