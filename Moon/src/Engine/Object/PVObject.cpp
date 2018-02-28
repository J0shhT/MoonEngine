#include "include/Engine/Object/PVObject.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::PVObject::PVObject()
{
	DEFINE_OBJECT_CONSTRUCTOR("PVObject");
	this->_size = Physics::Vector2(100.0, 100.0);
	this->_color = Graphics::Color3(1.0f, 1.0f, 1.0f);
	this->_isAnchored = false;
	this->_isSolid = true;
	this->_hasCollisionTop = false;
	this->_hasCollisionBottom = false;
	this->_hasCollisionLeft = false;
	this->_hasCollisionRight = false;
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
Physics::Vector2 Object::PVObject::GetSize() const
{
	return this->_size;
}
Physics::Vector2 Object::PVObject::GetVelocity() const
{
	return this->_velocity;
}
bool Object::PVObject::IsAnchored() const
{
	return this->_isAnchored;
}
bool Object::PVObject::IsSolid() const
{
	return this->_isSolid;
}
Graphics::Color3 Object::PVObject::GetColor() const
{
	return this->_color;
}
ContentId Object::PVObject::GetTextureContentId() const
{
	return this->_textureContentId;
}

//Member Setters
void Object::PVObject::SetPosition(Physics::Vector2 value)
{
	this->_position = value;
}
void Object::PVObject::SetSize(Physics::Vector2 value)
{
	this->_size = value;
}
void Object::PVObject::SetVelocity(Physics::Vector2 value)
{
	this->_velocity = value;
}
void Object::PVObject::SetAnchored(bool value)
{
	this->_isAnchored = value;
}
void Object::PVObject::SetSolid(bool value)
{
	this->_isSolid = value;
}
void Object::PVObject::SetColor(Graphics::Color3 value)
{
	this->_color = value;
}
bool Object::PVObject::SetTexture(std::string filePath)
{
	ContentId contentId = ContentProvider::singleton()->LoadFileTexture(filePath);
	this->_textureContentId = contentId;
	return contentId != "";
}

//Methods
void Object::PVObject::StepPhysics(double frameDeltaSec)
{
	if (this->IsAnchored()) { return; }
	Physics::Vector2 velocity = this->GetVelocity();
	Physics::Vector2 position = this->GetPosition();
	if (velocity.GetMagnitude() != 0.0)
	{
		double xPosition = position.GetX();
		double yPosition = position.GetY();
		double xVelocity = velocity.GetX();
		double yVelocity = velocity.GetY();

		//Additional collision check
		if (this->_hasCollisionTop && yVelocity < 0.0)
			yVelocity = 0.0;
		if (this->_hasCollisionBottom && yVelocity > 0.0)
			yVelocity = 0.0;
		if (this->_hasCollisionLeft && xVelocity < 0.0)
			xVelocity = 0.0;
		if (this->_hasCollisionRight && xVelocity > 0.0)
			xVelocity = 0.0;

		//Update position
		this->SetPosition(Physics::Vector2(
			xPosition + (xVelocity*frameDeltaSec),
			yPosition + (yVelocity*frameDeltaSec)
		));
	}
}
void Object::PVObject::CheckCollisions(std::vector<std::shared_ptr<PVObject>> physicsObjects)
{
	if (this->IsAnchored()) { return; }
	bool hasCollisionTop = false;
	bool hasCollisionBottom = false;
	bool hasCollisionRight = false;
	bool hasCollisionLeft = false;
	for (auto iter = physicsObjects.begin(); iter != physicsObjects.end(); iter++)
	{
		auto physicsObject = (*iter);
		if (physicsObject->IsSolid() && this->GetId() != physicsObject->GetId())
		{
			//Construct bounding box
			struct AABB {
				double x1;
				double x2;
				double y1;
				double y2;
			} a, b;
			a.x1 = this->GetPosition().GetX();
			a.x2 = this->GetPosition().GetX() + this->GetSize().GetX();
			a.y1 = this->GetPosition().GetY();
			a.y2 = this->GetPosition().GetY() + this->GetSize().GetY();
			b.x1 = physicsObject->GetPosition().GetX();
			b.x2 = physicsObject->GetPosition().GetX() + physicsObject->GetSize().GetX();
			b.y1 = physicsObject->GetPosition().GetY();
			b.y2 = physicsObject->GetPosition().GetY() + physicsObject->GetSize().GetY();
			//Overlap test
			bool isTouching = false;
			if (a.x2 >= b.x1 && a.x1 <= b.x2)
			{
				if (a.y2 >= b.y1 && a.y1 <= b.y2)
				{
					isTouching = true;
				}
			}
			if (isTouching)
			{
				//Side clamp
				double clampToRightJump = abs(a.x1 - b.x2);
				double clampToLeftJump = abs(a.x2 - b.x1);
				double clampToTopJump = abs(a.y2 - b.y1);
				double clampToBottomJump = abs(a.y1 - b.y2);
				auto clampJumps = { clampToRightJump, clampToLeftJump, clampToTopJump, clampToBottomJump };
				if ((std::min)(clampJumps) == clampToRightJump)
				{
					this->_position.SetX(b.x2);
					hasCollisionLeft = true;
				}
				else if ((std::min)(clampJumps) == clampToLeftJump)
				{
					this->_position.SetX(b.x1 - this->GetSize().GetX());
					hasCollisionRight = true;
				}
				else if ((std::min)(clampJumps) == clampToTopJump)
				{
					this->_position.SetY(b.y1 - this->GetSize().GetY());
					hasCollisionBottom = true;
				}
				else if ((std::min)(clampJumps) == clampToBottomJump)
				{
					this->_position.SetY(b.y2);
					hasCollisionTop = true;
				}
			}
		}
	}
	this->_hasCollisionTop = hasCollisionTop;
	this->_hasCollisionBottom = hasCollisionBottom;
	this->_hasCollisionLeft = hasCollisionLeft;
	this->_hasCollisionRight = hasCollisionRight;
}