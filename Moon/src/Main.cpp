#include <Lua/lua.hpp>

#include <string>
#include <iostream>

#include "include/Engine/Graphics/Window.h"

bool IsRunning = true;
unsigned int MouseX = 0U;
unsigned int MouseY = 0U;

int main(int argc, char** argv)
{

	Moon::Graphics::Window window("Moon Engine", 800, 600);

	//Init Lua 
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, "print('Hello World! Running ' .. _VERSION)");

	//Game loop
	while (IsRunning)
	{
		//Process events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				IsRunning = false;
				break;
			case SDL_MOUSEMOTION:
				MouseX = e.motion.x;
				MouseY = e.motion.y;
				glClearColor((float)MouseX / 800.0f, (float)MouseY / 600.0f, (float)MouseX / 800.0f, 1.0f);
				break;
			}
		}
		//Draw screen
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window.GetWindow());
	}

	//Close game
	SDL_Quit();
	lua_close(L);

	std::cout << "Game closed. Enter any character to continue..." << std::endl;
	char tmp;
	std::cin.get(tmp);

	return 0;
}