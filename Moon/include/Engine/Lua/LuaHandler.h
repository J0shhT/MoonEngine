/*

	Moon Engine - /Engine/Lua/LuaHandler.h

	Updated: April 4th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "include/Engine/ContentProvider.h"

#include <Lua/lua.hpp>

namespace Moon {

	namespace Lua {

		/*
			The Lua::Security enum describes a security context.

			Used by the Lua Engine in Moon Engine to tag certain scripts/code to allow them certain privelages
			when it comes to what functions it can access in the Moon Engine Lua API.
			Certain functions in our Lua API can only be accessed with certain security levels.

			GameScript = A normal executing script that a user made
			CommandLine = Code ran from the command line of Moon Engine
			MoonEngine = Official trusted code ran from the internal of Moon Engine
		*/
		enum Security {
			GameScript,
			CommandLine,
			MoonEngine
		};

		/*
			The Lua::LuaThread struct is a container that is meant to extend lua_State for more detail.

			Used by the Lua Engine in Moon Engine to tag certain scripts/code to allow them certain privelages
			when it comes to what functions it can access in the Moon Engine Lua API.
			Certain functions in our Lua API can only be accessed with certain security levels.

			state = The lua_State associated with this thread (pointer)
			securityContext = The security context for this thread (see Lua::Security enum)
			signature = A unique hash created by the code that is designated for this thread (for trust checks)
			id = A unique guid for this thread
		*/
		struct LuaThread {
			lua_State* state;
			Security securityContext;
			size_t signature;
			std::string id;
		};

		int ReadOnlyProperty(lua_State*);

	}

	/*
		The LuaHandler class is an object that handles all things Lua in Moon Engine,
		such as code execution, state & thread creation, thread management, etc
	*/
	class LuaHandler {

		public:
			LuaHandler();
			~LuaHandler() noexcept;

			//Singleton Getter
			static LuaHandler* singleton();

			//Methods
			int GetThreadCount() const;
			Lua::LuaThread GetThread(lua_State*) const;
			Lua::LuaThread GetThread(std::string threadId) const;
			Lua::LuaThread CreateThread(std::string& code, Lua::Security);
			void DeleteThread(Lua::LuaThread&);

			void ExecuteString(Lua::LuaThread&, std::string& code);
			void ExecuteString(std::string& code);

			bool RequireSecurityContext(Lua::LuaThread&, Lua::Security, std::string accessedItem) const;

		private:
			//Members
			static LuaHandler* instance;
			std::map<std::string, Lua::LuaThread> _threads;

			//Private Methods
			lua_State* _createLuaState() const;
			size_t _generateSignature(std::string& code);
			bool _verifySignature(std::string& code, Lua::LuaThread&);

			//Static Methods
			static int _error(lua_State*);
			static int _panic(lua_State*);

	};

}

/*
	Utility functions for things regarding Lua, such as information fetching.
*/

namespace Moon::Util {

	int GetLineNumber(lua_State*);
	std::string GetStackTraceback(lua_State*);
	void DumpStack(lua_State*);

}