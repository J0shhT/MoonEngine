#pragma once

#include "include/Common.h"

#include "include/Engine/ContentProvider.h"

#include <Lua/lua.hpp>

namespace Moon {

	namespace Lua {

		/*
			The Lua::Security enum describes a security context

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

		struct LuaThread {
			lua_State* state;
			Security securityContext;
			size_t signature;
			std::string id;
		};

	}

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

			bool RequireSecurityContext(lua_State*, Lua::Security, std::string accessedItem) const;

		private:
			//Members
			static LuaHandler* instance;
			std::map<std::string, Lua::LuaThread> _threads;

			//Private Methods
			lua_State* _createLuaState() const;
			size_t _generateSignature(std::string& code);
			bool _verifySignature(std::string& code, Lua::LuaThread&);

			//Static Methods
			static int _luaPanic(lua_State*);

	};

}