#pragma once
#include <string>
#include <future>

class Device
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
	~Device();

	bool start();
	bool stop();

	bool setState(bool st);
	bool setRate(int rt);
	bool setColor(const std::string& cl);

	bool        getState();
	int         getRate();
	std::string getColor();
};

