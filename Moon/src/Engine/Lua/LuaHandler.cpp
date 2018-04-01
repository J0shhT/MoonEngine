#include "include/Engine/Lua/LuaHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

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

//Singleton Getter
LuaHandler* LuaHandler::instance = nullptr;
LuaHandler* LuaHandler::singleton()
{
	assert(LuaHandler::instance != nullptr); //Make sure we never call :singleton() before it's made...
	return LuaHandler::instance;
}

//Methods
int LuaHandler::GetThreadCount() const
{
	int count = this->_threads.size();
	return ++count; //there will be a global thread, so we have to increment this
}
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
	return thread;
}
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
void LuaHandler::DeleteThread(Lua::LuaThread& thread)
{
	lua_close(this->_threads[thread.id].state);
	this->_threads.erase(thread.id);
}
void LuaHandler::ExecuteString(Lua::LuaThread& thread, std::string& code)
{
	if (this->_verifySignature(code, thread))
	{
		luaL_dostring(thread.state, code.c_str());
	}
	else
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Error, "LuaHandler::ExecuteString() - Trust check failed");
	}
}
void LuaHandler::ExecuteString(std::string& code)
{
	Lua::LuaThread thread = this->CreateThread(code, Lua::Security::CommandLine);
	this->ExecuteString(thread, code);
}


//Private Methods
lua_State* LuaHandler::_createLuaState() const
{
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);
	return state;
}
size_t LuaHandler::_generateSignature(std::string& code)
{
	boost::hash<std::string> codeHash;
	size_t hash = codeHash(code);
	return hash;
}
bool LuaHandler::_verifySignature(std::string& code, Lua::LuaThread& thread)
{
	size_t signature = this->_generateSignature(code);
	return thread.signature == signature;
}
