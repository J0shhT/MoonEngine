/*

	Moon Engine - /Engine/Lua/MoonDebugLibrary.cpp

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/MoonDebugLibrary.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

/*
	OpenLibrary

	Registers all MoonDebugLibrary functions into the specified lua_State
*/
void Lua::MoonDebugLibrary::OpenLibrary(lua_State* state)
{
	//Check whether or not the debug table already exists
	lua_getglobal(state, "debug");
	if (!lua_istable(state, -1))
	{
		lua_pop(state, 1);
		lua_newtable(state);
	}

	//debug.traceback()
	lua_pushcfunction(state, Lua::MoonDebugLibrary::traceback);
	lua_setfield(state, 1, "traceback");

	//debug.line()
	lua_pushcfunction(state, Lua::MoonDebugLibrary::line);
	lua_setfield(state, 1, "line");

	lua_setglobal(state, "debug");
}

/*
	**Lua Documentation**

	string debug.traceback()

	Returns a formatted string of the stack trace of the current thread, 
	which is the same as the one printed when an error occurs.
*/
int Lua::MoonDebugLibrary::traceback(lua_State *state)
{
	std::string traceback = Util::GetStackTraceback(state);
	lua_pushstring(state, traceback.c_str());
	return 1;
}

/*
	**Lua Documentation**

	int debug.line()

	Returns the line number that this function was called on.
*/
int Lua::MoonDebugLibrary::line(lua_State *state)
{
	int line = Util::GetLineNumber(state);
	lua_pushinteger(state, line);
	return 1;
}

