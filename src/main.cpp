#include "logic.h"

int main(int argc, char* argv[])
{
#ifdef __unix__
	std::string pipeName = "/tmp/pipe";
#elif defined _WIN32
	std::string pipeName = "\\\\.\\pipe\\pipe";
#endif
	
	if (argc > 1)
		pipeName = argv[1];

	Logic logic(pipeName);

	logic.run();

	return 0;
}