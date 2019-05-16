#include "device.h"
#include <iostream>
#include <chrono>

std::string Device::setState(std::string parameter)
{
	bool newState;

	if (parameter == "on")
		newState = true;
	else if (parameter == "off")
		newState = false;
	else
		return FAILED;
	
	m_ledState = newState;
	m_command.store(Command::Update);
	return OK + "\n";
}

std::string Device::setRate(std::string parameter)
{
	int newRate = 0;
	try
	{
		newRate = std::stoi(parameter);
	}
	catch (...)
	{
		return FAILED;
	}

	if (newRate < 0 || newRate > 5)
		return FAILED;
	
	m_ledRate = newRate;
	m_command.store(Command::Update);
	return OK + "\n";
}

std::string Device::setColor(std::string parameter)
{
	if (parameter != "red" &&
		parameter != "green" &&
		parameter != "blue")
		return FAILED;
	
	m_ledColor = parameter;
	m_command.store(Command::Update);
	return OK + "\n";
}

std::string Device::getState(std::string parameter)
{
	std::string result = OK + " ";
	result += m_ledState ? ON : OFF;
	return result + "\n";
}

std::string Device::getRate(std::string parameter)
{
	std::string result = OK + " ";
	result += std::to_string(m_ledRate);
	return result + "\n";
}

std::string Device::getColor(std::string parameter)
{
	std::string result = OK + " ";
	result += m_ledColor;
	return result + "\n";
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
	reflect();
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

