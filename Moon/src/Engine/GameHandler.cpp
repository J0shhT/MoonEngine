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
	assert(GameHandler::instance == nullptr); //Make sure we aren't making 2 instances (GameHandler is a singleton)...
	GameHandler::instance = this;

	//Create root game object that all other objects are children of
	this->_rootObject = std::make_shared<Object::Game>();
	this->_rootObject->SetParent(nullptr);

	//Create camera
	this->_camera = this->CreateGameObject<Object::Camera>(this->GetRootObject());
}

//Deconstructor
GameHandler::~GameHandler()
{
	
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
std::shared_ptr<Object::Camera> GameHandler::GetCamera() const
{
	return this->_camera;
}
GLuint GameHandler::GetShaderProgram() const
{
	return this->_shaderProgram;
}
GLuint GameHandler::GetVertexArrayObject() const
{
	return this->_glVAO;
}
glm::mat4 GameHandler::GetProjectionMatrix() const
{
	return this->_projectionMatrix;
}
glm::mat4 GameHandler::GetCameraMatrix() const
{
	return this->_cameraMatrix;
}
bool GameHandler::IsWireframe() const
{
	return this->_wireframeMode;
}

//Member Setters
void GameHandler::SetShaderProgram(GLuint program)
{
	this->_shaderProgram = program;
}
void GameHandler::SetWireframe(bool enabled)
{
	this->_wireframeMode = enabled;
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

			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					this->SetWireframe(!this->IsWireframe());
				}
				if (this->GetCamera()->GetCameraType() == Enum::CameraType::FreeCamera)
				{
					this->GetCamera()->KeyDownEvent(e.key.keysym.sym);
				}
				break;

			case SDL_KEYUP:
				if (this->GetCamera()->GetCameraType() == Enum::CameraType::FreeCamera)
				{
					this->GetCamera()->KeyUpEvent(e.key.keysym.sym);
				}
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
	//Update Camera
	this->GetCamera()->Update(frameDeltaSec);
}
void GameHandler::Render(double currentTime)
{
	//Initialize render-related things that only need to be done once
	static GLuint matrixId;
	float windowWidth = (float)this->GetTargetWindow()->GetWidth();
	float windowHeight = (float)this->GetTargetWindow()->GetHeight();
	static bool hasInitialized = false;
	if (!hasInitialized) {
		glCreateVertexArrays(1, &this->_glVAO);
		glBindVertexArray(this->_glVAO);
		glViewport(0, 0, windowWidth, windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-windowWidth / windowHeight, windowWidth / windowHeight, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		matrixId = glGetUniformLocation(this->GetShaderProgram(), "worldViewMatrix");
		this->_projectionMatrix = glm::ortho(0.0f, windowWidth, windowHeight, 0.0f);
		hasInitialized = true;
	}

	static const GLfloat clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, clearColor);

	//Shader Attribs
	GLfloat timeAttrib = (GLfloat)currentTime;
	glVertexAttrib1f(1, timeAttrib);

	this->_cameraMatrix = glm::translate(glm::mat4(), glm::vec3(
		-(float)this->GetCamera()->GetPosition().GetX(), (float)this->GetCamera()->GetPosition().GetY(), 0.0f
	));

	glUseProgram(this->GetShaderProgram());
	if (this->IsWireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Render game objects
	std::map<std::string, std::shared_ptr<Object::Object>>::iterator iter;
	for (iter = this->_gameObjects.begin(); iter != this->_gameObjects.end(); ++iter)
	{
		std::shared_ptr<Object::Object> object = iter->second;
		if (object->IsA<Object::Renderable>())
		{
			//std::cout << "GameHandler::Render() - Rendering " << object.get() << std::endl;
			std::shared_ptr<Object::Renderable> renderableObject = std::dynamic_pointer_cast<Object::Renderable>(object);
			object.reset();
			renderableObject->Render(matrixId);
		}
	}

	glUseProgram(0);
	glLoadIdentity();

	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(0.25f, 0.9f);
	std::string frameDeltaSecText = std::string("frameDeltaSec: ") + std::to_string(this->_lastFrameDeltaSec) + std::string("s");
	for (auto iter = frameDeltaSecText.begin(); iter != frameDeltaSecText.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *iter);
	}

	glRasterPos2f(-0.97f, -0.97f);
	std::string wireframeText = "Press spacebar to toggle wireframe";
	for (auto iter = wireframeText.begin(); iter != wireframeText.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *iter);
	}

	glRasterPos2f(-0.97f, -0.92f);
	std::string controlsText = "Use WASD to move the camera";
	for (auto iter = controlsText.begin(); iter != controlsText.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *iter);
	}

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