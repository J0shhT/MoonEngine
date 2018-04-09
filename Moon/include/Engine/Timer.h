/*

	Moon Engine - /Engine/Timer.h

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

namespace Moon {

	/*
		The Timer class is used to measure time elapses in a stopwatch-like manner.
	*/
	class Timer {

		public:
			Timer();
			~Timer() noexcept;

			//Singleton Getter
			static Timer* singleton();

			//Methods
			bool DoesTimerExist(std::string label);
			void StartTimer(std::string label);
			double StopTimer(std::string label);

		private:
			//Members
			static Timer* instance;
			std::map<std::string, std::chrono::steady_clock::time_point> _timePoints;

	};

}