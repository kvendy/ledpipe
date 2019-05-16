#include "logic.h"
#include <cstring>

int main(int argc, char* argv[])
{
	std::string pipeName = "/tmp/pipe";
	
	if (argc > 1)
	{
		if (strcmp(argv[1], "-g") != 0)
			pipeName = argv[1];
		else
		{
			Logic logic(pipeName);
			logic.createMethodsFile();
			return 0;
		}
	}

	Logic logic(pipeName);
	logic.run();

	return 0;
}