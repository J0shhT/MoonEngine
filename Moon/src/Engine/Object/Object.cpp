#include "include/Engine/Object/Object.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include "include/Engine/Game.h"
#include "include/Engine/StandardOut.h"
#include "include/Engine/Util.h"

using namespace Moon::Object;

//Constructor
Object::Object()
{
	DEFINE_OBJECT_CONSTRUCTOR();
	std::cout << "Object::Object() - " << this << std::endl;
}

//Destructor
Object::~Object()
{
	std::cout << "Object::~Object() - " << this << std::endl;
}

//Member Getters
std::string Object::GetType() const
{
	return this->_objectType;
}
std::string Object::GetId() const
{
	return this->_guid;
}
std::map<std::string, std::weak_ptr<Object>> Object::GetChildren() const
{
	return this->_children;
}
std::weak_ptr<Object> Object::GetParent() const
{
	return this->_parent;
}
std::string Object::GetName() const
{
	return this->_name;
}
bool Object::IsLocked() const
{
	return this->_isLocked;
}

//Member Setters
void Object::SetParent(std::shared_ptr<Object> parent)
{
	if (Moon::Util::IsWeakPtrInitialized<Object>(this->_parent))
	{
		//We are changing parents, remove us from the last parent's children
		std::shared_ptr<Object> oldParent = this->_parent.lock();
		if (oldParent != nullptr)
		{
			oldParent->RemoveChild(this->GetId());
		}
	}
	this->_parent = parent;
	if (parent != nullptr)
	{
		parent->AddChild(shared_from_this());
	}
}
void Object::SetName(std::string name)
{
	this->_name = name;
}
void Object::Lock()
{
	this->_isLocked = true;
}
void Object::Unlock()
{
	this->_isLocked = false;
}


//Methods
int Object::GetChildrenCount() const
{
	return this->_children.size();
}
std::shared_ptr<Object> Object::FindChildById(std::string id) const
{
	if (this->_children.count(id) <= 0)
	{
		return this->_children.at(id).lock();
	}
	return nullptr;
}
std::shared_ptr<Object> Object::FindFirstChild(std::string name) const
{
	for (auto it = this->_children.begin(); it != this->_children.end(); ++it)
	{
		//Iterate through children
		if (it->second.lock()->GetName() == name)
		{
			//Found a child that has the name specified, return in
			return it->second.lock();
		}
	}
	return nullptr;
}
bool Object::IsA(std::string type) const
{
	return (this->GetType() == type);
}
void Object::_forceDelete()
{
	if (Moon::Util::IsWeakPtrInitialized<Object>(this->_parent))
	{
		//We are attatched to a parent, delete us from the parent's children
		std::shared_ptr<Object> oldParent = this->_parent.lock();
		if (oldParent != nullptr)
		{
			oldParent->RemoveChild(this->GetId());
		}
	}
	this->_parent.reset(); //We no longer have a parent
	//Remove from lookup table
	if (Game::singleton()->_gameObjects.find(this->GetId()) != Game::singleton()->_gameObjects.end())
	{
		Game::singleton()->_gameObjects.at(this->GetId()).reset(); //free ptr ownership
		Game::singleton()->_gameObjects.erase(this->GetId()); //erase from lookup table
	}
}
void Object::Delete()
{
	//Locking an object prevents deletion
	if (!this->IsLocked())
	{
		this->_forceDelete();
	}
}


void Object::AddChild(std::shared_ptr<Object> child)
{
	this->_children[child->GetId()] = child;
}
void Object::RemoveChild(std::string guid)
{
	if (this->_children.find(guid) != this->_children.end())
	{
		this->_children.at(guid).reset();
		this->_children.erase(guid);
	}
}