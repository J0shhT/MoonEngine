#include <Lua/lua.hpp>

#include <string>
#include <iostream>

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/Game.h"
#include "include/Engine/StandardOut.h"

int main(int argc, char** argv)
{

	try
	{
		//Create window
		Moon::Graphics::Window window("Moon Engine", 800, 600);

		//Create game
		Moon::Game* game = new Moon::Game(window);

		//Init Lua
		/*
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaL_dostring(L, "print('Hello World! Running ' .. _VERSION)");
		*/

		//Game loop
		while (game->IsRunning())
		{
			game->ProcessEvents();
			game->Update();
			game->Render();
		}

		//Clean up Lua
		//lua_close(L);

		//Exit game
		game->Exit();
		delete game;
	}
	catch (const std::runtime_error& error)
	{
		Moon::StandardOut::Print<std::string>(Moon::StandardOut::OutputType::Error, error.what());
	}

	Moon::StandardOut::Print<std::string>(Moon::StandardOut::OutputType::Message, "Press enter to exit...");
	getchar();

	return 0;
}