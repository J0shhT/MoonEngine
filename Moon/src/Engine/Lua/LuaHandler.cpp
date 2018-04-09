/*

	Moon Engine - /Engine/Lua/LuaHandler.cpp

	Updated: April 5th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/LuaHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/Object/Script.h"

#include "include/Engine/Lua/MoonBaseExtension.h"
#include "include/Engine/Lua/MoonDebugLibrary.h"
#include "include/Engine/Lua/MoonTimeLibrary.h"

#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

using namespace Moon;

//Constructor
LuaHandler::LuaHandler()
{
	assert(LuaHandler::instance == nullptr); //Make sure we aren't making 2 instances (LuaHandler is a singleton)...
	LuaHandler::instance = this;
}

//Deconstructor
LuaHandler::~LuaHandler()
{
	
}

/**********************
	Singleton Getter
**********************/

LuaHandler* LuaHandler::instance = nullptr;
LuaHandler* LuaHandler::singleton()
{
	assert(LuaHandler::instance != nullptr); //Make sure we never call :singleton() before it's made...
	return LuaHandler::instance;
}

/*****************
	Methods
******************/

/*
	[public] GetThreadCount

	Returns the number of threads currently existing with Moon Engine.
*/
int LuaHandler::GetThreadCount() const
{
	return this->_threads.size();
}

/*
	[public] GetThread

	Returns the thread which corresponds to the specified lua_State.
*/
Lua::LuaThread LuaHandler::GetThread(lua_State* state) const
{
	Lua::LuaThread thread;
	for (auto iter = this->_threads.begin(); iter != this->_threads.end(); ++iter)
	{
		if (iter->second.state == state)
		{
			thread = iter->second;
			return thread;
		}
	}
	thread.state = nullptr;
	thread.signature = 0;
	thread.securityContext = Lua::Security::GameScript;
	thread.id = "";
	return thread;
}

/*
	[public] GetThread

	Returns the thread of the specified unique thread ID.
*/
Lua::LuaThread LuaHandler::GetThread(std::string threadId) const
{
	Lua::LuaThread thread;
	if (this->_threads.count(threadId) > 0)
	{
		thread = this->_threads.at(threadId);
		return thread;
	}
	thread.state = nullptr;
	thread.signature = 0;
	thread.securityContext = Lua::Security::GameScript;
	thread.id = "";
	return thread;
}

/*
	[public] CreateThread

	Creates a new thread with the specified code designation
	and security context, registers it with Moon Engine, and returns it.
*/
Lua::LuaThread LuaHandler::CreateThread(std::string& code, Lua::Security security)
{
	Lua::LuaThread thread;
	thread.securityContext = security;
	thread.state = this->_createLuaState();
	thread.signature = this->_generateSignature(code);
	thread.id = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
	this->_threads[thread.id] = thread;
	return thread;
}

/*
	[public] [API] DeleteThread

	Deletes the specified thread from Moon Engine, and does cleanup.
*/
void LuaHandler::DeleteThread(Lua::LuaThread& thread)
{
	lua_close(this->_threads[thread.id].state);
	this->_threads.erase(thread.id);
}

/*
	[public] ExecuteString

	This function takes the specified code and executes it in the specified thread.
	The specified thread must have been make unique for the specified code,
	as a signature trust check is done before execution.
*/
void LuaHandler::ExecuteString(Lua::LuaThread& thread, std::string& code)
{
	if (this->_verifySignature(code, thread))
	{
		const int loadSuccess = luaL_loadstring(thread.state, code.c_str());
		if (loadSuccess == 0)
		{
			//Loaded chunk successfully, call the chunk (with error handling)
			int hpos = lua_gettop(thread.state);
			lua_pushcfunction(thread.state, this->_error);
			lua_insert(thread.state, hpos);
			const int hasErrors = lua_pcall(thread.state, 0, 1, hpos);
			lua_remove(thread.state, hpos);
			if (hasErrors != 0) {
				lua_pop(thread.state, 1);
			}
			//We are done with this thread, script execution is finished
			this->DeleteThread(thread);
		}
		else
		{
			//Error occured loading chunk (syntax error, etc)
			auto script = Util::GetScriptObject(thread);
			std::string error;
			if (lua_isstring(thread.state, lua_gettop(thread.state)))
			{
				error = lua_tostring(thread.state, lua_gettop(thread.state));
			}
			if (script != nullptr)
			{
				if (error.substr(0, 1) == "[")
				{
					error = error.substr(error.find("\"]:") + 2, error.length());
				}
				error = script->GetName() + error;
			}
			StandardOut::Print<std::string>(StandardOut::OutputType::Error, error);
		}
	}
	else
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Error, "LuaHandler::ExecuteString() - Trust check failed");
	}
}

/*
	[public] ExecuteString

	This function takes the specified code and executes it in a
	brand new thread (with CommandLine security)
*/
void LuaHandler::ExecuteString(std::string& code)
{
	Lua::LuaThread thread = this->CreateThread(code, Lua::Security::CommandLine);
	this->ExecuteString(thread, code);
}

/*
	[public] RequireSecurityContext

	This function checks whether or not the specified thread
	is of or higher than the specified security context. If this check
	fails, a lua runtime error is raised and the function returns false.
*/
bool LuaHandler::RequireSecurityContext(Lua::LuaThread& thread, Lua::Security security, std::string accessedItem) const
{
	if (thread.securityContext < security)
	{
		//Security check failed
		lua_pushstring(thread.state, (std::string("Cannot access \"") + accessedItem + "\" (insufficient security context)").c_str());
		lua_error(thread.state);
		return false;
	}
	return true;
}


/**********************
	Private Methods
**********************/

/*
	[private] _createLuaState

	This function creates a new lua_State, populates the environment
	with the Lua 5.3 API and the extended Moon Engine Lua API, sandboxes the 
	environment with the neccessary security, and returns the lua_State for usage

	**This function is for internal usage by the engine, do not use manually**
*/
lua_State* LuaHandler::_createLuaState() const
{
	lua_State* state = luaL_newstate();
	lua_atpanic(state, &this->_panic);

	///Load Lua 5.3 API
	luaopen_base(state);
	lua_pop(state, 1);
	luaopen_coroutine(state);
	lua_pop(state, 1);
	luaopen_table(state);
	lua_pop(state, 1);
	luaopen_string(state);
	lua_pop(state, 1);
	luaopen_utf8(state);
	lua_pop(state, 1);
	luaopen_bit32(state);
	lua_pop(state, 1);
	luaopen_math(state);
	lua_pop(state, 1);

	///These libraries are dangerous and ruin sandbox, do not load them
	//luaopen_io(state);
	//luaopen_os(state);
	//luaopen_debug(state);
	//luaopen_package(state);

	///Remove dangerous base library functions
	lua_pushnil(state);
	lua_setglobal(state, "dofile");
	lua_pushnil(state);
	lua_setglobal(state, "load");
	lua_pushnil(state);
	lua_setglobal(state, "loadfile");

	///Load Moon Engine Lua API
	Lua::MoonBaseExtension::OpenLibrary(state);
	Lua::MoonDebugLibrary::OpenLibrary(state);
	Lua::MoonTimeLibrary::OpenLibrary(state);

	///Create new globals table
	lua_newtable(state);
	lua_setglobal(state, "_G");

	return state;
}

/*
	[private] _generateSignature

	This function generates and returns a hash of the specified code.

	**This function is for internal usage by the engine, do not use manually**
*/
size_t LuaHandler::_generateSignature(std::string& code)
{
	boost::hash<std::string> codeHash;
	size_t hash = codeHash(code);
	return hash;
}

/*
	[private] _verifySignature

	This function verifies (returns boolean) if the signature of the specified thread
	matches the signature that would be created from the specified code.

	**This function is for internal usage by the engine, do not use manually**
*/
bool LuaHandler::_verifySignature(std::string& code, Lua::LuaThread& thread)
{
	size_t signature = this->_generateSignature(code);
	return thread.signature == signature;
}

/**********************
	Static Methods
**********************/

/*
	[private] _error

	Error handler function for Lua. This is called by Lua when it
	encounters a runtime error. Do not call this function manually.

	**This function is for internal usage by the engine, do not use manually**
*/
int LuaHandler::_error(lua_State* state)
{
	//Format+create error message
	auto script = Util::GetScriptObject(state);
	std::string errorMessage = lua_tostring(state, 1);
	std::ostringstream errorStream;
	if (errorMessage.substr(0, 1) != "[")
	{
		errorStream << ":" << Util::GetLineNumber(state) << ": " << errorMessage;
		errorMessage = errorStream.str();
		errorStream.str("");
		errorStream.clear();
	}
	std::string formattedErrorMessage;
	if (errorMessage.substr(0, 1) == "[")
	{
		formattedErrorMessage = errorMessage.substr(errorMessage.find("\"]:") + 2, errorMessage.length());
	}
	else
	{
		formattedErrorMessage = errorMessage;
	}
	if (script != nullptr)
	{
		errorStream << script->GetName() << formattedErrorMessage;
	}
	else
	{
		errorStream << "(unknown)" << formattedErrorMessage;
	}
	errorMessage = errorStream.str();

	//Send error to output
	StandardOut::Print<std::string>(StandardOut::OutputType::Error, errorMessage);
	StandardOut::Print<std::string>(StandardOut::OutputType::Info, Util::GetStackTraceback(state));

	lua_pop(state, 1);
	lua_pushstring(state, errorMessage.c_str());
	return 1;
}

/*
	[private] _panic

	Panic handler function for Lua. This is called by Lua when it
	encounters an unprotected error. Do not call this function manually.

	**This function is for internal usage by the engine, do not use manually**
*/
int LuaHandler::_panic(lua_State* state)
{
	StandardOut::Print<std::string>(StandardOut::OutputType::Error, 
		std::string("Unprotected error in call to Lua API (") + std::string(lua_tostring(state, -1)) + std::string(")")
	);
	StandardOut::Print<std::string>(StandardOut::OutputType::Info, Util::GetStackTraceback(state));
	return 0;
}

/**********************
	Utility Functions
**********************/

/*
	GetLineNumber

	Returns the number of the last executed line of the specified lua_State.
	A value of 0 is returned if the line number can't be determined.
*/
int Util::GetLineNumber(lua_State* state)
{
	lua_Debug ar;
	if (lua_getstack(state, 1, &ar))
	{
		lua_getinfo(state, "nSl", &ar);
		if (ar.currentline != -1)
		{
			return ar.currentline;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
	GetStackTraceback

	Returns a formatted string describing the current call stack of the specified lua_State.
*/
std::string Util::GetStackTraceback(lua_State* state)
{
	auto script = Util::GetScriptObject(state);
	std::string scriptName;
	std::ostringstream stringStream;
	lua_Debug ar;
	int maxStackLevel = 12;
	stringStream << "Stack Begin";
	int level = 1;
	while (level < maxStackLevel && lua_getstack(state, level++, &ar)) {
		lua_getinfo(state, "nSl", &ar);
		if (script == nullptr) {
			scriptName = ar.short_src;
		}
		else
		{
			scriptName = script->GetName();
		}
		if (ar.currentline != -1) {
			if (ar.name) {
				stringStream << std::endl << "Script \"" << scriptName << "\", Line " << ar.currentline << " - " << ar.namewhat << " " << ar.name;
			}
			else {
				stringStream << std::endl << "Script \"" << scriptName << "\", Line " << ar.currentline;
			}
		}
	}
	stringStream << std::endl << "Stack End";
	return stringStream.str();
}

/*
	DumpStack

	For debugging purposes, prints the entire lua stack of the specified lua_State
*/
void Util::DumpStack(lua_State* state)
{
	const int size = lua_gettop(state);
	StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "// Lua Stack Debug //");
	if (size < 1) {
		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "(stack is empty)");
	}
	for (int i = 1; i <= size; i++)
	{
		std::ostringstream stackInfo;
		stackInfo << "[" << i << "] <" << lua_typename(state, lua_type(state, i)) << "> ";
		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, stackInfo.str());
	}
	StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "/////////////////////");
}