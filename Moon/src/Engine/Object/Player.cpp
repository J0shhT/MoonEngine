#include "include/Engine/Object/Player.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"

#include "include/Engine/Object/Rectangle.h"

using namespace Moon;

//Constructor
Object::Player::Player()
{
	DEFINE_OBJECT_CONSTRUCTOR("Player");
	this->SetIsControllable(false);
	this->SetWalkSpeed(125.0);
	this->_movementDirectionX = 0;
	this->_movementDirectionY = 0;
}

//Deconstructor
Object::Player::~Player()
{
	
}

//Member Getters
std::shared_ptr<Object::PVObject> Object::Player::GetCharacter() const
{
	return this->_character.lock();
}
double Object::Player::GetWalkSpeed() const
{
	return this->_walkSpeed;
}
bool Object::Player::IsControllable() const
{
	return this->_isControllable;
}

//Member Setters
void Object::Player::SetWalkSpeed(double value)
{
	this->_walkSpeed = value;
	this->_updateCharacterVelocity();
}
void Object::Player::SetIsControllable(bool value)
{
	this->_isControllable = value;
}

//Methods
void Object::Player::LoadCharacter()
{
	if (!Util::IsWeakPtrInitialized<PVObject>(this->_character))
	{
		GameHandler* game = GameHandler::singleton();
		auto character = game->CreateGameObject<Rectangle>(shared_from_this());
		character->SetName(this->GetName());
		character->SetSize({ 30, 70 });
		character->SetColor({ 0.0f, 0.0f, 0.0f });
		character->SetTexture("Content/Images/character.png");
		character->Lock(); //should we do this?
		this->_character = character;
		this->SetIsControllable(true);
	}
	else
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Warning, 
			"Player::LoadCharacter() - Character already exists"
		);
	}
}
void Object::Player::_updateCharacterVelocity()
{
	if (!Util::IsWeakPtrInitialized<PVObject>(this->_character)) { return; }
	std::shared_ptr<PVObject> character = this->GetCharacter();
	if (character != nullptr)
	{
		Physics::Vector2 velocity(
			this->GetWalkSpeed() * this->_movementDirectionX,
			this->GetWalkSpeed() * -this->_movementDirectionY
		);
		character->SetVelocity(velocity);
	}
	character.reset();
}

//Event functions
///These are called by GameHandler::ProcessEvents()
void Object::Player::KeyDownEvent(SDL_Keycode key)
{
	if (!this->IsControllable()) { return; }
	if (key == SDLK_w)
	{
		this->_movementDirectionY = 1;
		this->_updateCharacterVelocity();
	}
	else if (key == SDLK_a)
	{
		this->_movementDirectionX = -1;
		this->_updateCharacterVelocity();
	}
	else if (key == SDLK_s)
	{
		this->_movementDirectionY = -1;
		this->_updateCharacterVelocity();
	}
	else if (key == SDLK_d)
	{
		this->_movementDirectionX = 1;
		this->_updateCharacterVelocity();
	}
}
void Object::Player::KeyUpEvent(SDL_Keycode key)
{
	if (!this->IsControllable()) { return; }
	if (!this->IsControllable()) { return; }
	if (key == SDLK_w)
	{
		if (this->_movementDirectionY == 1)
		{
			this->_movementDirectionY = 0;
			this->_updateCharacterVelocity();
		}
	}
	else if (key == SDLK_a)
	{
		if (this->_movementDirectionX == -1)
		{
			this->_movementDirectionX = 0;
			this->_updateCharacterVelocity();
		}
	}
	else if (key == SDLK_s)
	{
		if (this->_movementDirectionY == -1)
		{
			this->_movementDirectionY = 0;
			this->_updateCharacterVelocity();
		}
	}
	else if (key == SDLK_d)
	{
		if (this->_movementDirectionX == 1)
		{
			this->_movementDirectionX = 0;
			this->_updateCharacterVelocity();
		}
	}
}
