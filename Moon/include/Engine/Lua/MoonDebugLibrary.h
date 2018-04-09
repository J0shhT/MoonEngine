/*

	Moon Engine - /Engine/Lua/MoonDebugLibrary.h

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "include/Engine/Lua/LuaHandler.h"

namespace Moon::Lua {

	/*
		The MoonDebugLibrary namespace contains the functions for the
		Moon Engine debug library that replaces the Lua 5.3 debug library.
	*/
	namespace MoonDebugLibrary {

		void OpenLibrary(lua_State*);

		//Lua API Functions
		int traceback(lua_State*);
		int line(lua_State*);

	}

}