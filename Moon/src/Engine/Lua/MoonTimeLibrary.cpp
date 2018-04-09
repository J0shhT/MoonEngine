/*

	Moon Engine - /Engine/Lua/MoonTimeLibrary.cpp

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/MoonTimeLibrary.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"
#include "include/Engine/Timer.h"

#include <SDL/SDL.h> //for SDL_GetTicks()

using namespace Moon;

/*
	OpenLibrary

	Registers all MoonTimeLibrary functions into the specified lua_State
*/
void Lua::MoonTimeLibrary::OpenLibrary(lua_State* state)
{
	//Check whether or not the time table already exists
	lua_getglobal(state, "time");
	if (!lua_istable(state, -1))
	{
		lua_pop(state, 1);
		lua_newtable(state);
	}

	//time.epoch()
	lua_pushcfunction(state, Lua::MoonTimeLibrary::epoch);
	lua_setfield(state, 1, "epoch");

	//time.elapsed()
	lua_pushcfunction(state, Lua::MoonTimeLibrary::elapsed);
	lua_setfield(state, 1, "elapsed");

	//time.frameDelta()
	lua_pushcfunction(state, Lua::MoonTimeLibrary::frameDelta);
	lua_setfield(state, 1, "frameDelta");

	//time.startTimer()
	lua_pushcfunction(state, Lua::MoonTimeLibrary::startTimer);
	lua_setfield(state, 1, "startTimer");

	//time.stopTimer()
	lua_pushcfunction(state, Lua::MoonTimeLibrary::stopTimer);
	lua_setfield(state, 1, "stopTimer");

	lua_setglobal(state, "time");
}

/*
	**Lua Documentation**

	int time.epoch()

	Returns the number of seconds elapsed since the Unix Epoch (January 1st, 1970). 
	This number does not include millisecond accuracy and returns a whole number only. 
	Use time.elapsed() if you need accurate millisecond time calculations.
*/
int Lua::MoonTimeLibrary::epoch(lua_State *state)
{
	time_t time = ::time(nullptr);
	long int seconds = static_cast<long int>(time);
	lua_pushinteger(state, seconds);
	return 1;
}

/*
	**Lua Documentation**

	number time.elapsed()

	Returns the number of seconds elapsed since the program started, with millisecond accuracy included.
*/
int Lua::MoonTimeLibrary::elapsed(lua_State *state)
{
	float seconds = static_cast<float>(SDL_GetTicks()) / 1000.0f;
	lua_pushnumber(state, seconds);
	return 1;
}

/*
	**Lua Documentation**

	number time.frameDelta()

	Returns how long it took to render the last frame in seconds.
*/
int Lua::MoonTimeLibrary::frameDelta(lua_State *state)
{
	double frameDelta = GameHandler::singleton()->GetFrameDelta();
	lua_pushnumber(state, frameDelta);
	return 1;
}

/*
	**Lua Documentation**

	void time.startTimer(string label)

	Starts a stopwatch-like timer with the specified label.
	Use time.stopTimer() with the same label to end the timer 
	and get the elapsed time between the start and end.
*/
int Lua::MoonTimeLibrary::startTimer(lua_State *state)
{
	Timer* timer = Timer::singleton();
	std::string label = luaL_optstring(state, 1, "nil");
	lua_pop(state, 1);
	if (!timer->DoesTimerExist(label))
	{
		timer->StartTimer(label);
		return 0;
	}
	else
	{
		return luaL_error(state, "Timer with the name \"%s\" already exists", label);
	}
}

/*
	**Lua Documentation**

	number time.stopTimer(string label)

	Stops the timer started with time.startTimer() that had the specified label. 
	This function returns, in seconds, how long was elapsed since the timer 
	was started (with millisecond accuracy).
*/
int Lua::MoonTimeLibrary::stopTimer(lua_State *state)
{
	Timer* timer = Timer::singleton();
	std::string label = luaL_optstring(state, 1, "nil");
	lua_pop(state, 1);
	if (timer->DoesTimerExist(label))
	{
		double elapsedTime = timer->StopTimer(label);
		lua_pushnumber(state, elapsedTime);
		return 1;
	}
	else
	{
		return luaL_error(state, "Timer with the name \"%s\" does not exist", label);
	}
}