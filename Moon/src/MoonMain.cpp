#include "include/Common.h"

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/GameHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/Object/Object.h"
#include "include/Engine/Object/Game.h"
#include "include/Engine/Object/Rectangle.h"

int main(int argc, char** argv)
{

	try
	{

		//Create window
		Moon::Graphics::Window* window = new Moon::Graphics::Window("Moon Engine", 800, 800);

		//Create game
		Moon::GameHandler* game = new Moon::GameHandler(window);

		//auto testObject1 = game->CreateGameObject<Moon::Object::Object>("TestObject1");
		//auto testObject2 = game->CreateGameObject<Moon::Object::Object>("TestObject2", testObject1);
		//DeleteGameObject(testObject2);

		auto testRectangle = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle");

		moonAppend(std::string, "Total Game Objects: ");
		moonPrint(int, game->GetGameObjectCount());

		moonAppend(std::string, "RootObject->GetName(): ");
		moonPrint(std::string, game->GetRootObject()->GetName());

		// Game loop ///////////////////////////////////
		auto frameLast = std::chrono::high_resolution_clock::now();
		while (game->IsRunning())
		{
			using namespace std::chrono;
			auto frameNow = high_resolution_clock::now();
			double frameDelta = static_cast<double>(duration_cast<microseconds>(frameNow - frameLast).count()) / 1000000.0;
			frameLast = high_resolution_clock::now();

			//Poll events
			game->ProcessEvents();

			//Update physics
			game->StepPhysics(frameDelta);

			//Render scene
			game->Render();
		}
		/////////////////////////////////////////////////

		moonAppend(std::string, "RootObject->GetAge(): ");
		moonPrint(double, game->GetRootObject()->GetAge());

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