#include "stdafx.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Lua/lua.hpp>

bool IsRunning = true;
unsigned int MouseX = 0U;
unsigned int MouseY = 0U;

int main(int argc, char** argv)
{

	//Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Moon Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

	//Init OpenGL
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	glewInit();
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

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
					glClearColor((float)MouseX/800.0f, (float)MouseY/600.0f, (float)MouseX/800.0f, 1.0f);
					break;
			}
		}
		//Draw screen
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
	}

	//Close game
	SDL_Quit();
	lua_close(L);

	std::cout << "Game closed. Enter any character to continue..." << std::endl;
	char tmp;
	std::cin.get(tmp);

    return 0;
}