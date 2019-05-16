#include "logic.h"

int main(int argc, char* argv[])
{
	std::string pipeName = "/tmp/pipe";
	
	if (argc > 1)
		pipeName = argv[1];

	Logic logic(pipeName);

	logic.run();

	return 0;
}