/*

	Moon Engine - /Engine/Object/Script.h

	Updated: April 5th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "Object.h"

#include "include/Engine/ContentProvider.h"
#include "include/Engine/Lua/LuaHandler.h"

namespace Moon::Object {

	/*
		The Script class is a Moon Engine game object whose purpose is
		to contain a representation of Lua code and to communicate to the 
		LuaHandler class to execute the contained code.
	*/
	class Script : public Object {

		public:
			Script();
			virtual ~Script() noexcept;

			//Member Getters
			bool IsLoaded() const;
			std::string GetThreadId() const;

			//Methods
			void LoadFromFile(std::string filePath);
			void LoadFromString(std::string& code);
			void Execute();

		protected:
			//Members
			bool _isLoaded;
			ContentId _scriptContentId;
			std::string _threadId;

	};

}

/*
	Utility functions for things regarding Script objects in Moon Engine.
*/

namespace Moon::Util {

	std::shared_ptr<Object::Script> GetScriptObject(Lua::LuaThread);
	std::shared_ptr<Object::Script> GetScriptObject(lua_State*);

}