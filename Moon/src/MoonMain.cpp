/*

	Moon Engine - /MoonMain.cpp

	Updated: April 5th, 2018
	Contributers: @J0shhT

	This is the main entry point file

*/

#include "include/Common.h"

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/Graphics/Shader.h"

#include "include/Engine/GameHandler.h"
#include "include/Engine/Lua/LuaHandler.h"
#include "include/Engine/SoundService.h"
#include "include/Engine/ContentProvider.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/Object/Object.h"
#include "include/Engine/Object/Game.h"
#include "include/Engine/Object/Rectangle.h"
#include "include/Engine/Object/Script.h"

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

//Main entry point
int main(int argc, char** argv)
{

	Moon::GameHandler* game = nullptr;

	try
	{

		std::string gameScript;

		// Command Line ///////////////////////
		///Options
		boost::program_options::options_description cmdLineDesc("Allowed options");
		cmdLineDesc.add_options()
			("help", "produce help message")
			("game", boost::program_options::value<std::string>(), "name of game to run");

		///Store selected options
		boost::program_options::variables_map cmdLine;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdLineDesc), cmdLine);
		boost::program_options::notify(cmdLine);

		///Parse options
		if (cmdLine.count("help")) {
			std::cout << cmdLineDesc << "\n";
			return 1;
		}
		if (cmdLine.count("game")) {
			gameScript = cmdLine["game"].as<std::string>();
		}
		else {
			#ifdef _DEBUG
				gameScript = "Debug";
			#else
				gameScript = "";
				throw std::runtime_error("Game not set, please use command line option --game to set the game");
			#endif
		}
		Moon::StandardOut::Print<std::string>(Moon::StandardOut::OutputType::Info,
			std::string("main() - Game set to \"") + gameScript + std::string("\"")
		);
		///////////////////////////////////////

		//Create window
		Moon::Graphics::Window* window = new Moon::Graphics::Window("Moon Engine", 800, 800);

		//Create game
		game = new Moon::GameHandler(window);

		//Load shaders
		GLuint vertexShader = Moon::Graphics::LoadVertexShader("Content/Shaders/Vertex.glsl");
		GLuint fragmentShader = Moon::Graphics::LoadFragmentShader("Content/Shaders/Fragment.glsl");
		//GLuint tessControlShader = Moon::Graphics::LoadControlShader("Content/Shaders/TesselationControl.glsl");
		//GLuint tessEvalShader = Moon::Graphics::LoadEvaluationShader("Content/Shaders/TesselationEvaluation.glsl");
		//GLuint geometryShader = Moon::Graphics::LoadGeometryShader("Content/Shaders/Geometry.glsl");

		GLuint shaderProgram = Moon::Graphics::LinkShaders(vertexShader, fragmentShader);
		game->SetShaderProgram(shaderProgram);

		//Load game script
		auto gameScriptObject = game->CreateGameObject<Moon::Object::Script>();
		gameScriptObject->Lock();
		gameScriptObject->SetName(gameScript);
		gameScriptObject->LoadFromFile(std::string("Content/Games/") + gameScript + std::string(".lua"));
		if (gameScriptObject->IsLoaded())
		{
			gameScriptObject->Execute();
		}
		else
		{
			//No point in continuing if the game script didn't even load
			game->Exit("Failed to load game script");
			delete game;
		}

		auto testRectangle1 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle1");
		testRectangle1->SetSize({ 250, 500 });
		testRectangle1->SetPosition({ 50, 50 });
		testRectangle1->SetAnchored(true);

		auto testRectangle2 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle2");
		testRectangle2->SetSize({ 350, 250 });
		testRectangle2->SetPosition({ 350, 100 });
		testRectangle2->SetAnchored(true);

		auto testRectangle3 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle3");
		testRectangle3->SetSize({ 500, 250 });
		testRectangle3->SetPosition({ 400, 600 });
		testRectangle3->SetAnchored(true);

		auto testRectangle4 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle4");
		testRectangle4->SetSize({ 100, 100 });
		testRectangle4->SetPosition({ 850, -50 });
		testRectangle4->SetVelocity({ 0, 75 });
		testRectangle4->SetTexture("Content/Images/spyro.png");
		testRectangle4->SetAnchored(false);

		auto testRectangle5 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle5");
		testRectangle5->SetSize({ 200, 200 });
		testRectangle5->SetPosition({ 400, 250 });
		testRectangle5->SetColor({ 1.0f, 0.0f, 0.0f });
		testRectangle5->SetAnchored(true);

		if (game->GetLocalPlayer()->GetCharacter() != nullptr)
		{
			game->GetLocalPlayer()->GetCharacter()->SetPosition({ 350, 400 });
		}

		game->GetTargetWindow()->DispalyWindow(true);

		Moon::SoundService::singleton()->PlaySound("Content/Sounds/oof.mp3", 0.5f);

		// Game loop ///////////////////////////////////
		auto frameLast = std::chrono::high_resolution_clock::now();
		while (game->IsRunning())
		{
			//Frame delta counter
			using namespace std::chrono;
			auto frameNow = high_resolution_clock::now();
			double frameDelta = static_cast<double>(duration_cast<microseconds>(frameNow - frameLast).count()) / 1000000.0;
			frameLast = high_resolution_clock::now();

			//Poll events
			game->ProcessEvents();

			//Update physics
			game->StepPhysics(frameDelta);

			//Render 
			double currentTime = (double)SDL_GetTicks() / 1000.0;
			game->Render(currentTime);

			//Update sound
			game->GetSoundSystem()->update();
		}
		/////////////////////////////////////////////////

		//Exit game
		game->Exit();
		delete game;
	}
	catch (const std::runtime_error& error)
	{
		Moon::StandardOut::Print<std::string>(Moon::StandardOut::OutputType::Error, std::string("[FATAL ERROR] ") + error.what());
		if (game != nullptr)
		{
			game->Exit();
			delete game;
		}
	}

	Moon::StandardOut::Print<std::string>(Moon::StandardOut::OutputType::Message, "Press enter to exit...");
	getchar();

	return 0;
}