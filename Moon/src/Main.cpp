#include <Lua/lua.hpp>

#include <string>
#include <iostream>

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/GameHandler.h"
#include "include/Engine/StandardOut.h"
#include "include/Engine/Object/Object.h"
#include "include/Engine/Object/Game.h"

int main(int argc, char** argv)
{

	try
	{

		//Create window
		Moon::Graphics::Window window("Moon Engine", 800, 600);

		//Create game
		Moon::GameHandler* game = new Moon::GameHandler(window);

		auto testObject1 = game->CreateGameObject<Moon::Object::Object>("TestObject1");
		auto testObject2 = game->CreateGameObject<Moon::Object::Object>("TestObject2", testObject1);
		DeleteGameObject(testObject2);

		append(std::string, "Total Game Objects: ");
		print(int, game->GetGameObjectCount());

		append(std::string, "rootObject.Name: ");
		print(std::string, game->GetRootObject()->GetName());

		//Game loop
		while (game->IsRunning())
		{
			game->ProcessEvents();
			game->Update();
			game->Render();
		}

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