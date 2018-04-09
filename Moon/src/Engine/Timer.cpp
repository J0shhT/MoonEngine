/*

	Moon Engine - /Engine/Timer.cpp

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Timer.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Timer::Timer()
{
	assert(Timer::instance == nullptr); //Make sure we aren't making 2 instances (Timer is a singleton)...
	Timer::instance = this;
}

//Deconstructor
Timer::~Timer()
{

}

/**********************
	Singleton Getter
**********************/

Timer* Timer::instance = nullptr;
Timer* Timer::singleton()
{
	assert(Timer::instance != nullptr); //Make sure we never call :singleton() before it's made...
	return Timer::instance;
}

/*****************
	Methods
******************/

bool Timer::DoesTimerExist(std::string label)
{
	return (this->_timePoints.count(label) > 0);
}

void Timer::StartTimer(std::string label)
{
	if (!this->DoesTimerExist(label))
	{
		this->_timePoints[label] = std::chrono::high_resolution_clock::now();
	}
	else
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Error,
			std::string("Timer::StartTimer() - Timer with the label \"") + label + std::string("\" already exists")
		);
	}
}

double Timer::StopTimer(std::string label)
{
	using namespace std::chrono;
	if (this->DoesTimerExist(label))
	{
		auto before = this->_timePoints.at(label);
		auto now = high_resolution_clock::now();
		this->_timePoints.erase(label);
		return (static_cast<double>(duration_cast<microseconds>(now - before).count()) / 1000000.0);
	}
	else
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Error,
			std::string("Timer::StopTimer() - Timer with the label \"") + label + std::string("\" does not exist")
		);
	}
}
