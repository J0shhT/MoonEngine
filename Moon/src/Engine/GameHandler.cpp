#include "include/Engine/GameHandler.h"

#include <assert.h>

#include "include/Engine/StandardOut.h"
#include "include/Engine/Object/Object.h"
#include "include/Engine/Object/Game.h"

using namespace Moon;

//Constructor
GameHandler::GameHandler(Graphics::Window& targetWindow):
	_isRunning(true),
	_targetWindow(targetWindow)
{
	assert(GameHandler::instance == nullptr); //Make sure we aren't making 2 instances (Game is a singleton)...
	GameHandler::instance = this;

	//Create root game object that all other objects are children of
	this->_rootObject = std::make_shared<Object::Game>();
	this->_rootObject->SetParent(nullptr);
}

//Deconstructor
GameHandler::~GameHandler()
{
	//todo: free all game objects
}

//Singleton Getter
GameHandler* GameHandler::instance = nullptr;
GameHandler* GameHandler::singleton()
{
	assert(GameHandler::instance != nullptr); //Make sure we never call :singleton() before it's made...
	return GameHandler::instance;
}

//Member Getters
bool GameHandler::IsRunning() const
{
	return this->_isRunning;
}
Graphics::Window& GameHandler::GetTargetWindow() const
{
	return this->_targetWindow;
}
std::shared_ptr<Object::Object> GameHandler::GetRootObject() const
{
	return this->_rootObject;
}

//Member Setters


//Methods
int GameHandler::GetGameObjectCount() const
{
	return this->_gameObjects.size();
}
void GameHandler::ProcessEvents()
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
void GameHandler::Update()
{

}
void GameHandler::Render()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(1, 1.0, 0.0, 0.0);
	glRotatef(1, 0.0, 1.0, 0.0);
	glRotatef(1, 0.0, 0.0, 1.0);

	glutWireSphere(0.5, 10, 10);

	SDL_GL_SwapWindow(this->GetTargetWindow().GetWindow());
}
void GameHandler::Exit(std::string error)
{
	static bool hasExited;
	if (!hasExited)
	{
		hasExited = true;

		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "GameHandler::Exit()");

		this->_isRunning = false;
		this->GetTargetWindow().DestroyWindow();
		SDL_Quit();

		if (error != "")
		{
			throw std::runtime_error(error);
		}
	}
}