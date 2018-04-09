/*

	Moon Engine - /Engine/Lua/Color3Bridge.h

	Updated: April 9th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "include/Engine/Lua/LuaHandler.h"

#include "include/Engine/Graphics/Color3.h"

namespace Moon::Lua {

	/*
		The Color3Bridge namespace contains the functions for the Moon Engine Color3 data type,
		and also includes utility functions for bridging between C++ and Lua with Color3.
	*/
	namespace Color3Bridge {

		static const char* Type = "Color3";

		void OpenLibrary(lua_State*);
		void RegisterType(lua_State*);

		void Push(lua_State*, Moon::Graphics::Color3*);
		Moon::Graphics::Color3* Get(lua_State*, int index);

		//Library Functions
		int constructor(lua_State*);
		int fromRGB(lua_State*);

		//Metamethods
		int __index(lua_State*);
		int __newindex(lua_State*);
		int __add(lua_State*);
		int __sub(lua_State*);
		int __mul(lua_State*);
		int __div(lua_State*);
		int __eq(lua_State*);
		int __tostring(lua_State*);

		//Methods
		///None

		//Getters
		int getR(lua_State*);
		int getG(lua_State*);
		int getB(lua_State*);

		//Setters
		///None

	}

}