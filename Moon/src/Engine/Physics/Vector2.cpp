#include "include/Engine/Physics/Vector2.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"

using namespace Moon::Physics;

//Constructors
Vector2::Vector2() : x(0.0), y(0.0) {
	
}
Vector2::Vector2(double x, double y) : x(x), y(y) { 
	
}
Vector2::Vector2(double xy) : x(xy), y(xy) { 
	
}

//Operators
Vector2& Vector2::operator=(const Vector2& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	return *this;
}
Vector2& Vector2::operator+=(const Vector2& other)
{
	this->x = this->x + other.x;
	this->y = this->y + other.y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& other)
{
	this->x = this->x - other.x;
	this->y = this->y - other.y;
	return *this;
}
Vector2& Vector2::operator*=(const Vector2& other)
{
	this->x = this->x * other.x;
	this->y = this->y * other.y;
	return *this;
}
Vector2& Vector2::operator/=(const Vector2& other)
{
	this->x = this->x / other.x;
	this->y = this->y / other.y;
	return *this;
}
bool Vector2::operator==(const Vector2& other) const
{
	return (this->GetMagnitude() == other.GetMagnitude());
}
bool Vector2::operator!=(const Vector2& other) const
{
	return !(*this == other);
}
bool Vector2::operator>(const Vector2& other) const
{
	return (this->GetMagnitude() > other.GetMagnitude());
}
bool Vector2::operator>=(const Vector2& other) const
{
	return (*this == other) || (*this > other);
}
bool Vector2::operator<(const Vector2& other) const
{
	return !(*this >= other);
}
bool Vector2::operator<=(const Vector2& other) const
{
	return !(*this > other);
}

//Getters
double Vector2::GetX() const
{
	return this->x;
}
double Vector2::GetY() const
{
	return this->y;
}
double Vector2::GetMagnitude() const
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}
double Vector2::GetArea() const
{
	return (this->x * this->y);
}
double Vector2::GetAngle(bool convertToDegrees) const
{
	if (convertToDegrees)
	{
		return atan(this->y / this->x) * (180.0 / boost::math::constants::pi<double>());
	}
	else {
		return atan(this->y / this->x);
	}
}
std::string Vector2::ToString() const
{
	return std::string("(") + std::to_string(this->x) + std::string(", ") + std::to_string(this->y) + std::string(")");
}

//Setters
void Vector2::SetX(double value)
{
	this->x = value;
}
void Vector2::SetY(double value)
{
	this->y = value;
}


//Util
GLCoordinates2f Moon::Physics::ConvertToGLCoordinates(const Moon::Physics::Vector2& vec2)
{
	GLCoordinates2f coords;
	Moon::Graphics::Window* window = Moon::GameHandler::singleton()->GetTargetWindow();
	float windowWidth = window->GetWidth();
	float windowHeight = window->GetHeight();
	coords.x = (static_cast<float>(vec2.GetX()) / (windowWidth / 2.0f)) - 1.0f;
	coords.y = (static_cast<float>(vec2.GetY()) / (windowHeight / 2.0f)) - 1.0f;
	return coords;
}