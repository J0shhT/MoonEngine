/*

	Moon Engine - /Engine/Lua/MoonBaseExtension.cpp

	Updated: April 4th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/MoonBaseExtension.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

/*
	OpenLibrary

	Registers all MoonBaseExtension functions into the specified lua_State
*/
void Lua::MoonBaseExtension::OpenLibrary(lua_State* state)
{
	lua_register(state, "print", Lua::MoonBaseExtension::print);
	lua_register(state, "warn", Lua::MoonBaseExtension::warn);
	lua_register(state, "identity", Lua::MoonBaseExtension::identity);
	lua_register(state, "version", Lua::MoonBaseExtension::version);
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

	string identity()

	Returns the security context identity of the current thread.
*/
int Lua::MoonBaseExtension::identity(lua_State *state)
{
	//TODO: change return type to an enum once we add enums to the Lua API
	Lua::LuaThread thread = LuaHandler::singleton()->GetThread(state);
	switch (thread.securityContext) {
		case Lua::Security::GameScript:
			lua_pushstring(state, "GameScript");
			break;
		case Lua::Security::CommandLine:
			lua_pushstring(state, "CommandLine");
			break;
		case Lua::Security::MoonEngine:
			lua_pushstring(state, "MoonEngine");
			break;
		default:
			lua_pushnil(state);
			break;
	}
	return 1;
}

/*
	**Lua Documentation**

	string version()

	Returns the current version of Moon Engine.
*/
int Lua::MoonBaseExtension::version(lua_State *state)
{
	lua_pushstring(state, MOON_VERSION);
	return 1;
}