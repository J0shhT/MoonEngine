#include "include/Engine/Game.h"

using namespace Moon;

//Constructor
Game::Game(Graphics::Window& targetWindow):
	_isRunning(true),
	_targetWindow(targetWindow)
{

}

//Deconstructor
Game::~Game()
{
	
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
	SDL_GL_SwapWindow(this->GetTargetWindow().GetWindow());
}
void Game::Exit(std::string error)
{
	static bool hasExited;
	if (!hasExited)
	{
		hasExited = true;
		this->_isRunning = false;
		this->GetTargetWindow().DestroyWindow();
		SDL_Quit();
		if (error != "")
		{
			std::cerr << "Game exited due to an encountered error: " << error << std::endl;
		}
		else
		{
			std::cout << "Game exited successfully" << std::endl;
		}
		std::cout << "Enter any character to continue..." << std::endl;
		getchar();
	}
}