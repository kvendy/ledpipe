#include "device.h"
#include <iostream>
#include <chrono>

#ifdef __unix__

const int RED   = 31;
const int GREEN = 32;
const int BLUE  = 34;

//\e[1G - move cursor to the 1st column
//\033[...m set text properties
//\033[0m - default

void setConColor(int ForgC)
{
	std::cout << "\033[" << ForgC << "m";
}

void gotoColumn(int x)
{
	std::cout << "\e[" << x <<"G";
}

void saveConAttr()
{
}

void clearConAttr()
{
	setConColor(0);
}

#elif defined _WIN32

#include <Windows.h>

const int RED   = 12;
const int GREEN = 10;
const int BLUE  = 9;

HANDLE hStdOut = INVALID_HANDLE_VALUE;
CONSOLE_SCREEN_BUFFER_INFO old_csbi;

void setConColor(int ForgC)
{
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;
	
	WORD wColor;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}

void gotoColumn(int x)
{
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;

	COORD coord;
	coord.X = x;
	coord.Y = old_csbi.dwCursorPosition.Y; //stay at the same line
	SetConsoleCursorPosition(hStdOut, coord);
}

void saveConAttr()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &old_csbi);
}

void clearConAttr()
{
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;
	
	WORD wColor = old_csbi.wAttributes;
	SetConsoleTextAttribute(hStdOut, wColor);
}

#endif // _WIN32


bool Device::setState(bool st)
{
	m_ledState = st;
	m_command.store(Command::Update);
	return true;
}

bool Device::setRate(int rt)
{
	if (rt < 0 || rt > 5)
		return false;
	
	m_ledRate = rt;
	m_command.store(Command::Update);
	return true;
}

bool Device::setColor(const std::string& cl)
{
	if (cl != "red" &&
		cl != "green" &&
		cl != "blue")
		return false;
	
	m_ledColor = cl;
	m_command.store(Command::Update);
	return true;
}

bool Device::getState()
{
	return m_ledState;
}

int Device::getRate()
{
	return m_ledRate;
}

std::string Device::getColor()
{
	return m_ledColor;
}

void Device::workingThread()
{
	bool        l_ledState = false;
	int         l_ledRate  = 1;
	std::string l_ledColor = "red";

	for (;;)
	{
		switch (m_command.load())
		{
		case None:
			break;
		case Stop:
			gotoColumn(0);
			return;
		case Update:
			m_mutexSettings.lock();
			l_ledState = m_ledState;
			l_ledRate  = m_ledRate;
			l_ledColor = m_ledColor;
			m_mutexSettings.unlock();
			m_command.store(None);
			break;
		}

		int colorCode = 0;
		if (l_ledColor == "red")
			colorCode = RED;
		else if (l_ledColor == "green")
			colorCode = GREEN;
		else if (l_ledColor == "blue")
			colorCode = BLUE;

		int ms = 1000 / 2 / l_ledRate;

		if (l_ledState)
		{
			gotoColumn(0);
			setConColor(colorCode);
			std::cout << "0" << std::flush;
			clearConAttr();
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			gotoColumn(0);
			std::cout << " " << std::flush;
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / l_ledRate));
	}
}

Device::Device() :
	m_ledState(false),
	m_ledRate(1),
	m_ledColor("red")
{
	saveConAttr();
}


Device::~Device()
{
}

bool Device::start()
{
	m_command.store(Command::Update);
	m_futureWork = std::async(std::launch::async, &Device::workingThread, this);
	return true;
}

bool Device::stop()
{
	m_command.store(Command::Stop);
	m_futureWork.wait();
	return true;
}

