#pragma once

#include "include/Common.h"

#include "Object.h"

#include "include/Engine/ContentProvider.h"
#include "include/Engine/Lua/LuaHandler.h"

namespace Moon::Object {

	class Script : public Object {

	public:
		Script();
		virtual ~Script() noexcept;

		//Member Getters
		bool IsLoaded() const;
		std::string GetThreadId() const;

		//Member Setters


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

namespace Moon::Util {

	std::shared_ptr<Object::Script> GetScriptObject(Lua::LuaThread);
	std::shared_ptr<Object::Script> GetScriptObject(lua_State*);

}