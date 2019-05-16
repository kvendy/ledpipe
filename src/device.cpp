#include "device.h"
#include <iostream>
#include <chrono>

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
	bool        l_ledState;
	int         l_ledRate;
	std::string l_ledColor;

	for (;;)
	{
		switch (m_command.load())
		{
		case None:
			break;
		case Stop:
			std::cout << "\e[1G" << std::flush;
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

		int colorCode = 30;
		if (l_ledColor == "red")
			colorCode = 31;
		else if (l_ledColor == "green")
			colorCode = 32;
		else if (l_ledColor == "blue")
			colorCode = 34;

		int ms = 1000 / 2 / l_ledRate;

		if (l_ledState)
		{
			//\e[1G - move cursor to the 1st column
			//\033[...m set text properties
			//\033[0m - default
			
			std::cout << "\e[1G\033[" << colorCode << "m\b0\033[0m" << std::flush;
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			std::cout << "\e[1G " << std::flush;
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

