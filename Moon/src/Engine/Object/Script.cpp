/*

	Moon Engine - /Engine/Object/Script.cpp

	Updated: April 5th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Object/Script.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"

using namespace Moon;

//Constructor
Object::Script::Script()
{
	DEFINE_OBJECT_CONSTRUCTOR("Script");
	this->_isLoaded = false;
	this->_threadId = "";
}

//Deconstructor
Object::Script::~Script()
{

}

/*****************
	Getters
******************/

/*
	[public] IsLoaded

	Returns whether or not the script object has been loaded with executable code.
*/
bool Object::Script::IsLoaded() const
{
	return this->_isLoaded;
}

/*
	[public] GetThreadId

	Returns the unique thread ID that is associated with this script object.
	A thread ID that is a blank string means there is no thread associated with this script.
*/
std::string Object::Script::GetThreadId() const
{
	return this->_threadId;
}

/*****************
	Methods
******************/

/*
	[public] LoadFromFile

	Loads the specified file's contents as executable Lua code for this script object.
*/
void Object::Script::LoadFromFile(std::string filePath)
{
	if (this->IsLoaded())
	{
		//Can't load a script object again once it's already loaded
		StandardOut::Print<std::string>(StandardOut::OutputType::Error, "Script::LoadFromFile() - Script is already loaded");
		return;
	}
	Moon::ContentId scriptContentId = Moon::ContentProvider::singleton()->LoadFileScript(filePath);
	if (scriptContentId != "")
	{
		this->_scriptContentId = scriptContentId;
		this->_isLoaded = true;
	}
}

/*
	[public] Execute

	Executes the currently loaded Lua code of this script object.
*/
void Object::Script::Execute()
{
	if (this->IsLoaded())
	{
		Moon::Content scriptContent = Moon::ContentProvider::singleton()->Get(this->_scriptContentId);
		Moon::Lua::LuaThread scriptThread;
		if (this->_threadId != "")
		{
			//This script object already has a thread registered for it, use the existing thread
			//TODO: Maybe we should just make a new thread on each execution?
			scriptThread = Moon::LuaHandler::singleton()->GetThread(this->_threadId);
		}
		else
		{
			//This script object does not have a thread registered yet, make one
			scriptThread = Moon::LuaHandler::singleton()->CreateThread(scriptContent.script_Data, Moon::Lua::Security::GameScript);
			this->_threadId = scriptThread.id;
		}
		if (scriptThread.state != nullptr)
		{
			//Execute code
			StandardOut::Print<std::string>(StandardOut::OutputType::Info,
				std::string("Script::Execute() - Executing Script \"") + this->GetName() + std::string("\"")
			);
			Moon::LuaHandler::singleton()->ExecuteString(scriptThread, scriptContent.script_Data);
		}
		else
		{
			//Uh oh, this shouldn't really ever happen?
			StandardOut::Print<std::string>(StandardOut::OutputType::Error, 
				std::string("Script::Execute() - Lua thread invalid (no state) for Script \"") + this->GetName() + std::string("\"")
			);
		}
	}
	else
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Error, 
			std::string("Script::Execute() - Script \"") + this->GetName() + std::string("\" is not loaded with a source")
		);
	}
}

/**********************
	Utility Functions
**********************/

/*
	GetScriptObject

	Attempts to find the script object that is associated with the
	specified Lua thread (through a search process). This function
	returns a nullptr if the script object is not found.
*/
std::shared_ptr<Object::Script> Util::GetScriptObject(Lua::LuaThread thread)
{
	auto scripts = GameHandler::singleton()->GetScriptObjects();
	for (auto iter = scripts.begin(); iter != scripts.end(); ++iter)
	{
		if (thread.id == (*iter)->GetThreadId())
		{
			return (*iter);
		}
	}
	return nullptr;
}

/*
	GetScriptObject

	Attempts to find the script object that is associated with the
	specified lua_State (through a search process). This function
	returns a nullptr if the script object is not found.
*/
std::shared_ptr<Object::Script> Util::GetScriptObject(lua_State* state)
{
	Lua::LuaThread thread = LuaHandler::singleton()->GetThread(state);
	if (thread.state != nullptr)
	{
		return Util::GetScriptObject(thread);
	}
	return nullptr;
}
