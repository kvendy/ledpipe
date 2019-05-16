#pragma once
#include "pipe_unix.h"
#include "device.h"

class Logic
{
	Pipe pipe;
	Device dev;
public:
	Logic(const std::string& pipeName);
	~Logic();

	void run();
};

