#pragma once
#include <string>
#include <future>

#include "isimplereflection.h"

const std::string OK     = "OK";
const std::string FAILED = "FAILED\n";
const std::string ON     = "on";
const std::string OFF    = "off";

class Device : public ISimpleReflection
{
	enum Command
	{
		None,
		Stop,
		Update
	};

	std::atomic<Command> m_command;

	bool        m_ledState;
	int         m_ledRate;
	std::string m_ledColor;
	
	bool m_runnig;
	std::mutex m_mutexSettings;
	std::future<void> m_futureWork;

	void workingThread();

public:
	Device();
	virtual ~Device();

	bool start();
	bool stop();

	std::string setState(std::string parameter);
	std::string setRate (std::string parameter);
	std::string setColor(std::string parameter);

	std::string getState(std::string parameter);
	std::string getRate (std::string parameter);
	std::string getColor(std::string parameter);

protected:
	virtual void reflect()
	{
		addMethod("set-led-state",
				  "LED on/off, argument: on, off",
				  std::bind(&Device::setState, this, std::placeholders::_1));
		addMethod("set-led-rate",
				  "Set LED flashing rate, argument: integer 0..5",
				  std::bind(&Device::setRate,  this, std::placeholders::_1));
		addMethod("set-led-color",
				  "Set LED color, argument: red, green or blue ",
				  std::bind(&Device::setColor, this, std::placeholders::_1));
		addMethod("get-led-state",
				  "Get LED state, no arguments",
				  std::bind(&Device::getState, this, std::placeholders::_1));
		addMethod("get-led-rate",
				  "Get LED flashing rate, no arguments ",
				  std::bind(&Device::getRate,  this, std::placeholders::_1));
		addMethod("get-led-color",
				  "Get LED color, no arguments",
				  std::bind(&Device::getColor, this, std::placeholders::_1));
	}
};

