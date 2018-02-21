#include "include/Engine/Object/Rectangle.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::Rectangle::Rectangle()
{
	DEFINE_OBJECT_CONSTRUCTOR("Rectangle");
	this->_size = Physics::Vector2(100.0, 100.0);
}

//Deconstructor
Object::Rectangle::~Rectangle()
{

}

//Member Getters
Physics::Vector2 Object::Rectangle::GetSize() const
{
	return this->_size;
}

//Member Setters
void Object::Rectangle::SetSize(Physics::Vector2 value)
{
	this->_size = value;
}

//Methods
void Object::Rectangle::Render()
{
	Physics::GLCoordinates2f position = Physics::ConvertToGLCoordinates(this->GetPosition());
	Physics::GLCoordinates2f size = Physics::ConvertToGLCoordinates(this->GetSize());

	//This is required
	//Todo: add type of Physics::ConvertToGLCoordinates() for size
	size.x += 1;
	size.y += 1;

	glBegin(GL_QUADS);
		//todo: add color
		glColor3f(1.0f, 1.0f, 1.0f);
		//top-left
		glVertex2f(position.x, -position.y);
		//bottom-left
		glVertex2f(position.x, -position.y - size.y);
		//bottom-right
		glVertex2f(position.x + size.x, -position.y - size.y);
		//top-right
		glVertex2f(position.x + size.x, -position.y);
	glEnd();

}
void Object::Rectangle::StepPhysics(double frameDeltaSec)
{
	if (this->IsAnchored()) { return; }
	Physics::Vector2 velocity = this->GetVelocity();
	Physics::Vector2 position = this->GetPosition();
	double rotVelocity = this->GetRotVelocity();
	double rotation = this->GetRotation();
	if (velocity.GetMagnitude() != 0.0)
	{
		double xPosition = position.GetX();
		double yPosition = position.GetY();
		double xVelocity = velocity.GetX();
		double yVelocity = velocity.GetY();
		this->SetPosition(Physics::Vector2(
			xPosition + (xVelocity*frameDeltaSec), 
			yPosition + (yVelocity*frameDeltaSec)
		));
	}
	if (this->GetRotVelocity() != 0.0)
	{
		this->SetRotation(rotation + (rotVelocity*frameDeltaSec));
	}
}