#include "include/Engine/GameHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/Object/Game.h"

using namespace Moon;

//Constructor
GameHandler::GameHandler(Graphics::Window* targetWindow):
	_isRunning(true),
	_targetWindow(targetWindow)
{
	assert(GameHandler::instance == nullptr); //Make sure we aren't making 2 instances (Game is a singleton)...
	GameHandler::instance = this;

	glViewport(0, 0, targetWindow->GetWidth(), targetWindow->GetHeight());
	glCreateVertexArrays(1, &this->_glVAO);
	glBindVertexArray(this->_glVAO);

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
Graphics::Window* GameHandler::GetTargetWindow() const
{
	return this->_targetWindow;
}
std::shared_ptr<Object::Object> GameHandler::GetRootObject() const
{
	return this->_rootObject;
}
GLuint GameHandler::GetShaderProgram()
{
	return this->_shaderProgram;
}
GLuint GameHandler::GetVertexArrayObject()
{
	return this->_glVAO;
}

//Member Setters
void GameHandler::SetShaderProgram(GLuint program)
{
	this->_shaderProgram = program;
}

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
void GameHandler::StepPhysics(double frameDeltaSec)
{
	this->_lastFrameDeltaSec = frameDeltaSec;
	//Step PVObjects
	std::map<std::string, std::shared_ptr<Object::Object>>::iterator iter;
	for (iter = this->_gameObjects.begin(); iter != this->_gameObjects.end(); ++iter)
	{
		std::shared_ptr<Object::Object> object = iter->second;
		if (object->IsA<Object::PVObject>())
		{
			//std::cout << "GameHandler::StepPhysics() - Stepping " << object.get() << std::endl;
			std::shared_ptr<Object::PVObject> physicsObject = std::dynamic_pointer_cast<Object::PVObject>(object);
			object.reset();
			if (!physicsObject->IsAnchored())
			{
				physicsObject->StepPhysics(frameDeltaSec);
			}
		}
	}
}
void GameHandler::Render(double currentTime)
{
	static const GLfloat clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, clearColor);

	//Render game objects
	/*
	std::map<std::string, std::shared_ptr<Object::Object>>::iterator iter;
	for (iter = this->_gameObjects.begin(); iter != this->_gameObjects.end(); ++iter)
	{
		std::shared_ptr<Object::Object> object = iter->second;
		if (object->IsA<Object::Renderable>())
		{
			//std::cout << "GameHandler::Render() - Rendering " << object.get() << std::endl;
			std::shared_ptr<Object::Renderable> renderableObject = std::dynamic_pointer_cast<Object::Renderable>(object);
			object.reset();
			renderableObject->Render();
		}
	}
	*/

	glUseProgram(this->GetShaderProgram());

	//Attribs
	GLfloat posAttrib[] = {
		(float)sin(currentTime) * 0.5f,
		(float)cos(currentTime) * 0.6f,
		0.0f,
		0.0f
	};
	GLfloat colorAttrib[] = {
		0.5f * (float)sin(currentTime - (3.141592f / 2.0f)) + 0.5f,
		0.5f * (float)cos(currentTime - (3.141592f / 2.0f)) + 0.5f,
		0.0f,
		0.0f
	};
	GLfloat timeAttrib = (GLfloat)currentTime;
	glVertexAttrib4fv(0, posAttrib);
	glVertexAttrib1f(1, timeAttrib);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 6);

	// FrameDeltaSec debug text display //
	/*
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f(0.0f, 0.8f);
	std::string text = std::string("frameDeltaSec: ") + std::to_string(this->_lastFrameDeltaSec) + std::string("s");
	for (auto iter = text.begin(); iter != text.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *iter);
	}
	//////////////////////////////////////
	*/

	SDL_GL_SwapWindow(this->GetTargetWindow()->GetWindow());
}
void GameHandler::Exit(std::string error)
{
	static bool hasExited;
	if (!hasExited)
	{
		hasExited = true;

		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "GameHandler::Exit()");

		this->_isRunning = false;
		this->GetTargetWindow()->DestroyWindow();
		delete this->GetTargetWindow();

		glDeleteVertexArrays(1, &this->_glVAO);
		glDeleteProgram(this->_shaderProgram);
		SDL_Quit();


		if (error != "")
		{
			throw std::runtime_error(error);
		}
	}
}