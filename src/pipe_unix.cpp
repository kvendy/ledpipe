#include "pipe_unix.h"

#include <sys/types.h>
#include <sys/stat.h>

Pipe::Pipe(const std::string& name) :
	pipeName(name)
{
	mkfifo(pipeName.c_str(), 0666);
}

Pipe::~Pipe()
{
	remove(pipeName.c_str());
}

std::string Pipe::read()
{
	std::ifstream ifs(pipeName);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	return content;
}

void Pipe::write(const std::string& data)
{
	std::ofstream ifs(pipeName);
	ifs << data;
}
