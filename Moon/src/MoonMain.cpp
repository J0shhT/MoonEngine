#include "include/Common.h"

#include "include/Engine/Graphics/Window.h"
#include "include/Engine/Graphics/Shader.h"
#include "include/Engine/GameHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/Object/Object.h"
#include "include/Engine/Object/Game.h"
#include "include/Engine/Object/Rectangle.h"

int main(int argc, char** argv)
{

	Moon::GameHandler* game = nullptr;

	try
	{

		//Create window
		Moon::Graphics::Window* window = new Moon::Graphics::Window("Moon Engine", 800, 800);

		//Create game
		game = new Moon::GameHandler(window);

		//Load shaders
		#ifdef _DEBUG
			GLuint vertexShader = Moon::Graphics::LoadVertexShader(DEBUG_DIR + std::string("Content/Shaders/Vertex.glsl"));
			GLuint fragmentShader = Moon::Graphics::LoadFragmentShader(DEBUG_DIR + std::string("Content/Shaders/Fragment.glsl"));
			//GLuint tessControlShader = Moon::Graphics::LoadControlShader(DEBUG_DIR + std::string("Content/Shaders/TesselationControl.glsl"));
			//GLuint tessEvalShader = Moon::Graphics::LoadEvaluationShader(DEBUG_DIR + std::string("Content/Shaders/TesselationEvaluation.glsl"));
			//GLuint geometryShader = Moon::Graphics::LoadGeometryShader(DEBUG_DIR + std::string("Content/Shaders/Geometry.glsl"));
		#else
			GLuint vertexShader = Moon::Graphics::LoadVertexShader("Content/Shaders/Vertex.glsl");
			GLuint fragmentShader = Moon::Graphics::LoadFragmentShader("Content/Shaders/Fragment.glsl");
			//GLuint tessControlShader = Moon::Graphics::LoadControlShader("Content/Shaders/TesselationControl.glsl");
			//GLuint tessEvalShader = Moon::Graphics::LoadEvaluationShader("Content/Shaders/TesselationEvaluation.glsl");
			//GLuint geometryShader = Moon::Graphics::LoadGeometryShader("Content/Shaders/Geometry.glsl");
		#endif
		GLuint shaderProgram = Moon::Graphics::LinkShaders(vertexShader, fragmentShader);
		game->SetShaderProgram(shaderProgram);

		auto testRectangle1 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle1");
		testRectangle1->SetSize({250, 500});
		testRectangle1->SetPosition({50, 50});
		testRectangle1->SetTexture("Content/Images/emoji.png");

		auto testRectangle2 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle2");
		testRectangle2->SetSize({ 250, 250 });
		testRectangle2->SetPosition({ 350, 100 });
		testRectangle2->SetTexture("Content/Images/sky.png");

		auto testRectangle3 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle3");
		testRectangle3->SetSize({ 500, 250 });
		testRectangle3->SetPosition({ 400, 600 });
		testRectangle3->SetTexture("Content/Images/space.jpg");

		auto testRectangle4 = game->CreateGameObject<Moon::Object::Rectangle>("TestRectangle4");
		testRectangle4->SetSize({ 100, 100 });
		testRectangle4->SetPosition({ 600, 400 });
		testRectangle4->SetTexture("Content/Images/spyro.png");

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

			//Render 
			double currentTime = (double)SDL_GetTicks() / 1000.0;
			game->Render(currentTime);
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