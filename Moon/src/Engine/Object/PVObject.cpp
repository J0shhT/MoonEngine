#include "include/Engine/Object/PVObject.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::PVObject::PVObject()
{
	DEFINE_OBJECT_CONSTRUCTOR("PVObject");
	this->_isAnchored = false;
	this->_isSolid = true;
}

//Deconstructor
Object::PVObject::~PVObject()
{

}

//Member Getters
Physics::Vector2 Object::PVObject::GetPosition() const
{
	return this->_position;
}
double Object::PVObject::GetRotation() const
{
	return this->_rotation;
}
Physics::Vector2 Object::PVObject::GetVelocity() const
{
	return this->_velocity;
}
double Object::PVObject::GetRotVelocity() const
{
	return this->_rotVelocity;
}
bool Object::PVObject::IsAnchored() const
{
	return this->_isAnchored;
}
bool Object::PVObject::IsSolid() const
{
	return this->_isSolid;
}

//Member Setters
void Object::PVObject::SetPosition(Physics::Vector2 value)
{
	this->_position = value;
}
void Object::PVObject::SetRotation(double value)
{
	this->_rotation = value;
}
void Object::PVObject::SetVelocity(Physics::Vector2 value)
{
	this->_velocity = value;
}
void Object::PVObject::SetRotVelocity(double value)
{
	this->_rotVelocity = value;
}
void Object::PVObject::SetAnchored(bool value)
{
	this->_isAnchored = value;
}
void Object::PVObject::SetSolid(bool value)
{
	this->_isSolid = value;
}
