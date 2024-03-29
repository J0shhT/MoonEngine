#include "include/Engine/Object/Object.h"

#include "include/Engine/GameHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon::Object;

bool Object::_isInstanced = false;

//Constructor
Object::Object()
{
	DEFINE_OBJECT_CONSTRUCTOR("Object");
	Moon::StandardOut::Append<std::string>(Moon::StandardOut::OutputType::Debug, "Object::Object() - ");
	Moon::StandardOut::Print<Object*>(Moon::StandardOut::OutputType::Debug, this);
}

//Destructor
Object::~Object()
{
	Moon::StandardOut::Append<std::string>(Moon::StandardOut::OutputType::Debug, "Object::~Object() - ");
	Moon::StandardOut::Print<Object*>(Moon::StandardOut::OutputType::Debug, this);
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
std::shared_ptr<Object> Object::GetParent() const
{
	return this->_parent.lock();
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
	if (this->IsLocked()) { return; } //Locking prevents parent change
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
double Object::GetAge() const
{
	auto now = std::chrono::high_resolution_clock::now();
	return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - this->_createdAt).count()) / 1000000.0;
}
std::shared_ptr<Object> Object::FindChildById(std::string id) const
{
	if (this->_children.count(id) > 0)
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
	if (GameHandler::singleton()->_gameObjects.find(this->GetId()) != GameHandler::singleton()->_gameObjects.end())
	{
		GameHandler::singleton()->_gameObjects.at(this->GetId()).reset(); //free ptr ownership
		GameHandler::singleton()->_gameObjects.erase(this->GetId()); //erase from lookup table
	}
}
bool Object::Delete()
{
	//Locking an object prevents deletion
	if (!this->IsLocked())
	{
		this->_forceDelete();
		return true;
	}
	else {
		return false;
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