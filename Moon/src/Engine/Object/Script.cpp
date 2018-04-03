#include "include/Engine/Object/Script.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"

using namespace Moon;

//Constructor
Object::Script::Script()
{
	DEFINE_OBJECT_CONSTRUCTOR("Script");
	_isLoaded = false;
}

//Deconstructor
Object::Script::~Script()
{

}

//Member Getters
bool Object::Script::IsLoaded() const
{
	return this->_isLoaded;
}

//Member Setters


//Methods
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
		Moon::Content scriptContent = Moon::ContentProvider::singleton()->Get(scriptContentId);
		Moon::Lua::LuaThread scriptThread = Moon::LuaHandler::singleton()->CreateThread(scriptContent.script_Data, Moon::Lua::Security::GameScript);
		this->_scriptContentId = scriptContentId;
		this->_threadId = scriptThread.id;
		this->_isLoaded = true;
	}
}
void Object::Script::Execute()
{
	if (this->IsLoaded())
	{
		Moon::Content scriptContent = Moon::ContentProvider::singleton()->Get(this->_scriptContentId);
		Lua::LuaThread scriptThread = Moon::LuaHandler::singleton()->GetThread(this->_threadId);
		if (scriptThread.state != nullptr)
		{
			StandardOut::Print<std::string>(StandardOut::OutputType::Info,
				std::string("Script::Execute() - Executing Script \"") + this->GetName() + std::string("\"")
			);
			Moon::LuaHandler::singleton()->ExecuteString(scriptThread, scriptContent.script_Data);
		}
		else
		{
			StandardOut::Print<std::string>(StandardOut::OutputType::Error, 
				std::string("Script::Execute() - Lua thread invalid for Script \"") + this->GetName() + std::string("\"")
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
