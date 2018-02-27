#include "include/Engine/Object/Camera.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::Camera::Camera()
{
	DEFINE_OBJECT_CONSTRUCTOR("Camera");
	this->SetMovementSpeed(200.0f);
	this->SetCameraType(Enum::CameraType::FreeCamera);
}

//Deconstructor
Object::Camera::~Camera()
{
	
}

//Member Getters
Enum::CameraType Object::Camera::GetCameraType() const
{
	return this->_cameraType;
}
Physics::Vector2 Object::Camera::GetPosition() const
{
	return this->_position;
}
double Object::Camera::GetMovementSpeed() const
{
	return this->_speed;
}
Physics::Vector2 Object::Camera::GetMovementDirection() const
{
	return Physics::Vector2((double)this->_movementDirectionX, (double)this->_movementDirectionY);
}
char Object::Camera::GetMovementDirectionX() const
{
	return this->_movementDirectionX;
}
char Object::Camera::GetMovementDirectionY() const
{
	return this->_movementDirectionY;
}
std::shared_ptr<Object::PVObject> Object::Camera::GetTarget() const
{
	return this->_target.lock();
}

//Member Setters
void Object::Camera::SetCameraType(Enum::CameraType type)
{
	this->_cameraType = type;
	this->SetMovementDirection(0, 0);
}
void Object::Camera::SetPosition(Physics::Vector2 position)
{
	this->_position = position;
}
void Object::Camera::SetMovementSpeed(double value)
{
	this->_speed = value;
}
void Object::Camera::SetMovementDirection(char x, char y)
{
	assert(x <= 1 && x >= -1 && y <= 1 && y >= -1);
	this->_movementDirectionX = x;
	this->_movementDirectionY = y;
}
void Object::Camera::SetMovementDirectionX(char value)
{
	assert(value <= 1 && value >= -1);
	this->_movementDirectionX = value;
}
void Object::Camera::SetMovementDirectionY(char value)
{
	assert(value <= 1 && value >= -1);
	this->_movementDirectionY = value;
}
void Object::Camera::SetTarget(std::shared_ptr<PVObject> target)
{
	this->_target = target;
}

//Methods
void Object::Camera::Update(double frameDeltaSec)
{
	if (this->GetCameraType() == Enum::CameraType::Scriptable)
	{ 
		return; 
	}
	else if (this->GetCameraType() == Enum::CameraType::FreeCamera)
	{
		Physics::Vector2 cameraVelocity = this->GetVelocity();
		Physics::Vector2 cameraPosition = this->GetPosition();
		if (cameraVelocity.GetMagnitude() != 0.0)
		{
			this->SetPosition(Physics::Vector2(
				cameraPosition.GetX() + (cameraVelocity.GetX()*frameDeltaSec),
				cameraPosition.GetY() + (cameraVelocity.GetY()*frameDeltaSec)
			));
		}
	}
	else if (this->GetCameraType() == Enum::CameraType::Attach)
	{
		if (Util::IsWeakPtrInitialized<PVObject>(this->_target)) {
			this->SetPosition(this->GetTarget()->GetPosition());
		}
	}
}
Physics::Vector2 Object::Camera::GetVelocity() const
{
	return Physics::Vector2((double)this->_movementDirectionX * this->_speed, (double)this->_movementDirectionY * this->_speed);
}

//Event functions
///These are called by GameHandler::ProcessEvents()
void Object::Camera::KeyDownEvent(SDL_Keycode key)
{
	if (this->GetCameraType() != Enum::CameraType::FreeCamera) { return; }
	if (key == SDLK_w)
	{
		this->SetMovementDirectionY(-1);
	}
	else if (key == SDLK_a)
	{
		this->SetMovementDirectionX(-1);
	}
	else if (key == SDLK_s)
	{
		this->SetMovementDirectionY(1);
	}
	else if (key == SDLK_d)
	{
		this->SetMovementDirectionX(1);
	}
}
void Object::Camera::KeyUpEvent(SDL_Keycode key)
{
	if (this->GetCameraType() != Enum::CameraType::FreeCamera) { return; }
	if (key == SDLK_w)
	{
		if (this->_movementDirectionY == -1)
			this->SetMovementDirectionY(0);
	}
	else if (key == SDLK_a)
	{
		if (this->_movementDirectionX == -1)
			this->SetMovementDirectionX(0);
	}
	else if (key == SDLK_s)
	{
		if (this->_movementDirectionY == 1)
			this->SetMovementDirectionY(0);
	}
	else if (key == SDLK_d)
	{
		if (this->_movementDirectionX == 1)
			this->SetMovementDirectionX(0);
	}
}
