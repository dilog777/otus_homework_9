#pragma once

#include <string>



class ConsoleLogger
{
public:
	ConsoleLogger();
	~ConsoleLogger();
	
	void log(time_t time, const std::string &str);

private:
	class Impl;
	Impl *_impl;
};