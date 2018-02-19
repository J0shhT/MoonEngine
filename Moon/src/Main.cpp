#include <Lua/lua.hpp>

#include <string>
#include <iostream>

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/Game.h"
#include "include/Engine/StandardOut.h"
#include "include/Engine/Object/Object.h"

int main(int argc, char** argv)
{

	try
	{
		//Create window
		Moon::Graphics::Window window("Moon Engine", 800, 600);

		//Create game
		Moon::Game* game = new Moon::Game(window);

		auto testObject = game->CreateGameObject<Moon::Object::Object>();
		testObject->SetName("Test");

		append(std::string, "testObject memory address: ");
		print(Moon::Object::Object*, game->GetRootObject()->FindFirstChild("Test").get());

		append(std::string, "Total Game Objects: ");
		print(int, game->GetGameObjectCount());

		append(std::string, "rootObject ptr_use_count: ");
		print(long, game->GetRootObject().use_count());

		append(std::string, "testObject ptr_use_count: ");
		print(long, testObject.use_count());

		append(std::string, "rootObject children count: ");
		print(int, game->GetRootObject()->GetChildrenCount());

		append(std::string, "testObject children count: ");
		print(int, testObject->GetChildrenCount());

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