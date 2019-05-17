#pragma once
#ifdef __unix__
#include "pipe_unix.h"
#elif defined _WIN32
#include "pipe_windows.h"
#endif

#include "device.h"

class Logic
{
	Pipe pipe;
	Device dev;
public:
	Logic(const std::string& pipeName);
	~Logic();

	void run();
	std::string call(const std::string& fn, const std::string& param);
};

