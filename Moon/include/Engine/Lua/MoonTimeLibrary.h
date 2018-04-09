/*

	Moon Engine - /Engine/Lua/MoonTimeLibrary.h

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE SOURCE (.cpp) FILE FOR DOCUMENTATION OF FUNCTIONS**

*/

#pragma once

#include "include/Common.h"

#include "include/Engine/Lua/LuaHandler.h"

namespace Moon::Lua {

	/*
		The MoonTimeLibrary namespace contains the functions for the Moon Engine time library.
	*/
	namespace MoonTimeLibrary {

		void OpenLibrary(lua_State*);

		//Lua API Functions
		int epoch(lua_State*);
		int elapsed(lua_State*);
		int frameDelta(lua_State*);
		int startTimer(lua_State*);
		int stopTimer(lua_State*);

	}

}