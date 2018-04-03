/*

	Moon Engine - /Engine/Lua/MoonBaseExtension.h

	Updated: April 3rd, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "include/Engine/Lua/LuaHandler.h"

namespace Moon::Lua {

	/*
		The MoonBaseExtension namespace contains the functions for the 
		base Moon Engine API that extends on the existing Lua 5.3 base library.
	*/
	namespace MoonBaseExtension {

		void OpenLibrary(lua_State*);

		//Lua API Functions
		int print(lua_State*);
		int warn(lua_State*);
		int tick(lua_State*);

	}

}