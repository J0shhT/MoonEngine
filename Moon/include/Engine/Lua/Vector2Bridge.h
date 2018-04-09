/*

	Moon Engine - /Engine/Lua/Vector2Bridge.h

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "include/Engine/Lua/LuaHandler.h"

#include "include/Engine/Physics/Vector2.h"

namespace Moon::Lua {

	/*
		The Vector2Bridge namespace contains the functions for the Moon Engine Vector2 data type,
		and also includes utility functions for bridging between C++ and Lua with Vector2.
	*/
	namespace Vector2Bridge {

		static const char* Type = "Vector2";

		void OpenLibrary(lua_State*);
		void RegisterType(lua_State*);

		void Push(lua_State*, Moon::Physics::Vector2*);
		Moon::Physics::Vector2* Get(lua_State*, int index);

		//Library Functions
		int constructor(lua_State*);

		//Type Metamethods
		int __index(lua_State*);
		int __newindex(lua_State*);
		int __add(lua_State*);
		int __sub(lua_State*);
		int __mul(lua_State*);
		int __div(lua_State*);
		int __unm(lua_State*);
		int __eq(lua_State*);
		int __lt(lua_State*);
		int __le(lua_State*);
		int __tostring(lua_State*);

		//Type Methods
		///None

		//Type Getters
		int getX(lua_State*);
		int getY(lua_State*);
		int getMagnitude(lua_State*);
		int getArea(lua_State*);
		int getAngle(lua_State*);

		//Type Setters
		///None

	}

}