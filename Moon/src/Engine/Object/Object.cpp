#include "include/Engine/Object/Object.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include "include/Engine/Util.h"

using namespace Moon::Object;

//Constructor
Object::Object()
{
	DEFINE_OBJECT_CONSTRUCTOR()
	std::cout << "Object created (" << this << ")" << std::endl;
}

//Destructor
Object::~Object()
{
	std::cout << "Object destroyed (" << this << ")" << std::endl;
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

//Member Setters
void Object::SetParent(std::shared_ptr<Object> parent)
{
	if (!Moon::Util::IsWeakPtrInitialized<Object>(this->_parent))
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
std::shared_ptr<Object> Moon::Object::Object::FindFirstChild(std::string name) const
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


void Object::AddChild(std::shared_ptr<Object> child)
{
	this->_children[child->GetId()] = child;
}
void Object::RemoveChild(std::string guid)
{
	if (this->_children.count(guid) <= 0)
	{
		this->_children.erase(guid);
	}
}