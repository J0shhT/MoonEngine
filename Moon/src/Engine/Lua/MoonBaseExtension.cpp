/*

	Moon Engine - /Engine/Lua/MoonBaseExtension.cpp

	Updated: April 3rd, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/MoonBaseExtension.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include <stdio.h>
#include <time.h>

using namespace Moon;

/*
	OpenLibrary

	Registers all MoonBaseLibrary functions into the specified lua_State
*/
void Lua::MoonBaseExtension::OpenLibrary(lua_State* state)
{
	lua_register(state, "print", Lua::MoonBaseExtension::print);
	lua_register(state, "warn", Lua::MoonBaseExtension::warn);
	lua_register(state, "tick", Lua::MoonBaseExtension::tick);
}

/*
	**Lua Documentation**

	void print(string message, ...)
	Prints the specified message(s) to the output
*/
int Lua::MoonBaseExtension::print(lua_State *state)
{
	std::string message;
	int n = lua_gettop(state);
	lua_getglobal(state, "tostring");
	for (int i = 1; i <= n; i++)
	{
		if (i > 1)
		{
			message = message + ' ';
		}
		lua_pushvalue(state, -1);
		lua_pushvalue(state, i);
		lua_call(state, 1, 1);
		const char* s = luaL_checkstring(state, -1);
		message = message + s;
		lua_pop(state, 1);
	}
	StandardOut::Print<std::string>(StandardOut::OutputType::Message, message);
	return 0;
}

/*
	**Lua Documentation**

	void warn(string message, ...)
	Prints the specified message(s) to the output as a warning
*/
int Lua::MoonBaseExtension::warn(lua_State *state)
{
	std::string message;
	int n = lua_gettop(state);
	lua_getglobal(state, "tostring");
	for (int i = 1; i <= n; i++)
	{
		if (i > 1)
		{
			message = message + ' ';
		}
		lua_pushvalue(state, -1);
		lua_pushvalue(state, i);
		lua_call(state, 1, 1);
		const char* s = luaL_checkstring(state, -1);
		message = message + s;
		lua_pop(state, 1);
	}
	StandardOut::Print<std::string>(StandardOut::OutputType::Warning, message);
	return 0;
}

/*
	**Lua Documentation**

	int tick()
	Returns the number of seconds elapsed since the Unix Epoch (January 1st, 1970)
*/
int Lua::MoonBaseExtension::tick(lua_State *state)
{
	time_t time = ::time(nullptr);
	long int seconds = static_cast<long int>(time);
	lua_pushinteger(state, seconds);
	return 1;
}