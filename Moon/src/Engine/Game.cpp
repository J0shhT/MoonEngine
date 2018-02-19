#include "include/Engine/Game.h"

#include <assert.h>

#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Game::Game(Graphics::Window& targetWindow):
	_isRunning(true),
	_targetWindow(targetWindow)
{
	assert(Game::instance == nullptr); //Make sure we aren't making 2 instances (Game is a singleton)...
	Game::instance = this;
}

//Deconstructor
Game::~Game()
{
	
}

//Singleton Getter
Game* Game::instance = nullptr;
Game* Game::singleton()
{
	return Game::instance;
}

//Member Getters
bool Game::IsRunning() const
{
	return this->_isRunning;
}
Graphics::Window& Game::GetTargetWindow() const
{
	return this->_targetWindow;
}

//Member Setters


//Methods
void Game::ProcessEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			this->Exit();
			break;
		case SDL_MOUSEMOTION:
			//todo
			break;
		}
	}
}
void Game::Update()
{

}
void Game::Render()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(1, 1.0, 0.0, 0.0);
	glRotatef(1, 0.0, 1.0, 0.0);
	glRotatef(1, 0.0, 0.0, 1.0);

	glutWireTeapot(0.5);

	SDL_GL_SwapWindow(this->GetTargetWindow().GetWindow());
}
void Game::Exit(std::string error)
{
	static bool hasExited;
	if (!hasExited)
	{
		hasExited = true;

		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "Game::Exit()");

		this->_isRunning = false;
		this->GetTargetWindow().DestroyWindow();
		SDL_Quit();

		if (error != "")
		{
			throw std::runtime_error(error);
		}
	}
}