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


		//Member Setters


		//Methods
		

	protected:
		//Members
		bool _disabled;
		ContentId _scriptContentId;
		lua_State* _thread;

	};

}