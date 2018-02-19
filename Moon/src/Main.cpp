#include <Lua/lua.hpp>

#include <string>
#include <iostream>

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/Game.h"

int main(int argc, char** argv)
{

	//Create window
	Moon::Graphics::Window window("Moon Engine", 800, 600);

	//Create game
	Moon::Game game(window);

	//Init Lua
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, "print('Hello World! Running ' .. _VERSION)");

	//Game loop
	while (game.IsRunning())
	{
		game.ProcessEvents();
		game.Update();
		game.Render();
	}

	//Clean up Lua
	lua_close(L);

	//Exit game
	game.Exit();

	return 0;
}