

#include "include/Engine/GameHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/Object/Game.h"

#include "include/Engine/ContentProvider.h"
#include "include/Engine/SoundService.h"


using namespace Moon;

//Constructor
GameHandler::GameHandler(Graphics::Window* targetWindow):
	_isRunning(true),
	_targetWindow(targetWindow)
{
	assert(GameHandler::instance == nullptr); //Make sure we aren't making 2 instances (GameHandler is a singleton)...
	GameHandler::instance = this;

	//Initialize singletons
	new ContentProvider();
	new SoundService();

	//Load FMOD Sound System
	FMOD::System_Create(&this->_soundSystem);
	this->_soundSystem->init(32, FMOD_INIT_NORMAL, NULL);

	//Create root game object that all other objects are children of
	this->_rootObject = std::make_shared<Object::Game>();
	this->_rootObject->SetParent(nullptr);
	this->_rootObject->Lock();

	//Create local player
	this->_localPlayer = this->CreateGameObject<Object::Player>(this->GetRootObject());
	this->_localPlayer->SetName("LocalPlayer");
	this->_localPlayer->Lock();
	this->_localPlayer->LoadCharacter();

	//Create camera
	this->_camera = this->CreateGameObject<Object::Camera>(this->GetRootObject());
	this->_camera->SetName("CurrentCamera");
	if (this->_localPlayer->GetCharacter() != nullptr) {
		this->_camera->SetTarget(this->_localPlayer->GetCharacter());
		this->_camera->SetCameraType(Enum::CameraType::Attach);
	}
	this->_camera->Lock();
}

//Deconstructor
GameHandler::~GameHandler()
{
	delete ContentProvider::singleton();
	delete SoundService::singleton();
	this->_soundSystem->close();
	this->_soundSystem->release();
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
FMOD::System* GameHandler::GetSoundSystem() const
{
	return this->_soundSystem;
}
std::shared_ptr<Object::Object> GameHandler::GetRootObject() const
{
	return this->_rootObject;
}
std::shared_ptr<Object::Camera> GameHandler::GetCamera() const
{
	return this->_camera;
}
std::shared_ptr<Object::Player> GameHandler::GetLocalPlayer() const
{
	return this->_localPlayer;
}
std::vector<std::shared_ptr<Object::PVObject>> GameHandler::GetPhysicsObjects() const
{
	std::vector<std::shared_ptr<Object::PVObject>> objects;
	for (auto iter = this->_gameObjects.begin(); iter != this->_gameObjects.end(); ++iter)
	{
		std::shared_ptr<Object::Object> object = iter->second;
		if (object->IsA<Object::PVObject>())
		{
			std::shared_ptr<Object::PVObject> physicsObject = std::dynamic_pointer_cast<Object::PVObject>(object);
			objects.emplace_back(physicsObject);
			object.reset();
		}
	}
	return objects;
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
				if (this->GetLocalPlayer() != nullptr)
				{
					if (this->GetLocalPlayer()->IsControllable())
					{
						this->GetLocalPlayer()->KeyDownEvent(e.key.keysym.sym);
					}
				}
				break;

			case SDL_KEYUP:
				if (this->GetCamera()->GetCameraType() == Enum::CameraType::FreeCamera)
				{
					this->GetCamera()->KeyUpEvent(e.key.keysym.sym);
				}
				if (this->GetLocalPlayer() != nullptr)
				{
					if (this->GetLocalPlayer()->IsControllable())
					{
						this->GetLocalPlayer()->KeyUpEvent(e.key.keysym.sym);
					}
				}
				break;
		}
	}
}
void GameHandler::StepPhysics(double frameDeltaSec)
{
	this->_lastFrameDeltaSec = frameDeltaSec;
	auto physicsObjects = this->GetPhysicsObjects();
	for (auto iter = physicsObjects.begin(); iter != physicsObjects.end(); ++iter)
	{
		std::shared_ptr<Object::PVObject> object = (*iter);
		if (!object->IsAnchored())
		{
			//Check collisons
			object->CheckCollisions(physicsObjects);

			//Tick object
			object->StepPhysics(frameDeltaSec);
		}
		object.reset();
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCreateVertexArrays(1, &this->_glVAO);
		glBindVertexArray(this->_glVAO);
		glViewport(0, 0, windowWidth, windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-windowWidth / windowHeight, windowWidth / windowHeight, -1.0, 1.0);
		matrixId = glGetUniformLocation(this->GetShaderProgram(), "worldViewMatrix");
		this->_projectionMatrix = glm::ortho(0.0f, windowWidth, windowHeight, 0.0f);
		glMatrixMode(GL_MODELVIEW);
		hasInitialized = true;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	//Shader Attribs
	//GLfloat timeAttrib = (GLfloat)currentTime;
	//glVertexAttrib1f(3, timeAttrib);

	this->_cameraMatrix = glm::translate(glm::mat4(), glm::vec3(
		-(float)this->GetCamera()->GetPosition().GetX() + windowWidth/2, 
		-(float)this->GetCamera()->GetPosition().GetY() + windowHeight/2, 
		0.0f
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
	std::vector<std::weak_ptr<Object::Renderable>> renderAfterObjects;
	for (auto iter = this->_gameObjects.begin(); iter != this->_gameObjects.end(); ++iter)
	{
		std::shared_ptr<Object::Object> object = iter->second;
		if (object->IsA<Object::Renderable>())
		{
			std::shared_ptr<Object::Renderable> renderableObject = std::dynamic_pointer_cast<Object::Renderable>(object);
			object.reset();
			if (renderableObject->GetParent()->IsA<Object::Player>())
			{
				//We render players after so they are on top
				//todo: add zindex to avoid this hardcoding?
				renderAfterObjects.emplace_back(renderableObject);
			}
			else
			{
				//std::cout << "GameHandler::Render() - Rendering " << object.get() << std::endl;
				renderableObject->Render(this->_shaderProgram, matrixId);
			}
		}
	}
	//TODO: make this better...
	for (auto iter = renderAfterObjects.begin(); iter != renderAfterObjects.end(); ++iter)
	{
		//std::cout << "GameHandler::Render() - Rendering " << object.get() << std::endl;
		iter->lock()->Render(this->_shaderProgram, matrixId);
	}

	glUseProgram(0);
	glLoadIdentity();

	/* DEBUG GUIS */
	glColor3f(1.0f, 0.0f, 1.0f);
	glRasterPos2f(-0.97, 0.87f);
	std::string frameDeltaSecText = std::to_string(this->_lastFrameDeltaSec) + std::string("s");
	for (auto iter = frameDeltaSecText.begin(); iter != frameDeltaSecText.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *iter);
	}
	glRasterPos2f(-0.97, 0.93f);
	std::string fpsCounterText = std::to_string(static_cast<int>(1.0/this->_lastFrameDeltaSec)) + std::string(" FPS");
	for (auto iter = fpsCounterText.begin(); iter != fpsCounterText.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *iter);
	}

	/* CONTROLS HELP */
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-0.97f, -0.97f);
	std::string wireframeText = "Press spacebar to toggle wireframe";
	for (auto iter = wireframeText.begin(); iter != wireframeText.end(); iter++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *iter);
	}
	glRasterPos2f(-0.97f, -0.92f);
	std::string controlsText = "Use WASD to move your character";
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