#include "ConsoleLogger.h"

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>



class ConsoleLogger::Impl
{
public:
	Impl();
	~Impl();

	void log(const std::string &str);

private:
	std::condition_variable _condition;
	std::queue<std::string> _strings;
	std::mutex _mutex;
	std::thread _thread;
	std::atomic<bool> _stopped { false };

	void workerThread();
	void processString(const std::string &str) const;
};



ConsoleLogger::Impl::Impl()
{
	_thread = std::thread(&Impl::workerThread, this);
}



ConsoleLogger::Impl::~Impl()
{
	{
		std::unique_lock lock { _mutex };
		_stopped = true;
	}

	_condition.notify_all();
	_thread.join();
}



void ConsoleLogger::Impl::log(const std::string &str)
{
	{
		std::unique_lock lock { _mutex };
		_strings.push(str);
	}

	_condition.notify_one();
}



void ConsoleLogger::Impl::workerThread()
{
	std::unique_lock lock { _mutex };

	while (!_stopped)
	{
		while (!_stopped && _strings.empty())
		{
			_condition.wait(lock);
		}

		while (!_strings.empty())
		{
			auto str = _strings.front();
			_strings.pop();

			lock.unlock();
			processString(str);
			lock.lock();
		}
	}
}



void ConsoleLogger::Impl::processString(const std::string &str) const
{
	std::cout << str << std::endl;
}



ConsoleLogger::ConsoleLogger()
	: _impl { new Impl }
{
}



ConsoleLogger::~ConsoleLogger()
{
	delete _impl;
}



void ConsoleLogger::log([[maybe_unused]] time_t time, const std::string &str)
{
	_impl->log(str);
}
