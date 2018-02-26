#include "include/Engine/Graphics/Color3.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon::Graphics;

//Constructors
Color3::Color3() : _r(1.0f), _g(1.0f), _b(1.0f)
{

}
Color3::Color3(unsigned char r, unsigned char g, unsigned char b):
	_r((float)r/255), _g((float)g/255), _b((float)b/255)
{
	
}
Color3::Color3(unsigned char rgb):
	_r((float)rgb/255), _g((float)rgb/255), _b((float)rgb/255)
{

}
Color3::Color3(float r, float g, float b): _r(r), _g(g), _b(b)
{

}
Color3::Color3(float rgb) : _r(rgb), _g(rgb), _b(rgb)
{

}

//Operators
Color3& Color3::operator=(const Color3& rhs)
{
	this->_r = rhs._r;
	this->_g = rhs._g;
	this->_b = rhs._b;
	return *this;
}
Color3& Color3::operator+=(const Color3& other)
{
	this->_r = this->_r + other._r;
	this->_g = this->_g + other._g;
	this->_b = this->_b + other._b;
	return *this;
}
Color3& Color3::operator-=(const Color3& other)
{
	this->_r = this->_r - other._r;
	this->_g = this->_g - other._g;
	this->_b = this->_b - other._b;
	return *this;
}
Color3& Color3::operator*=(const Color3& other)
{
	this->_r = this->_r * other._r;
	this->_g = this->_g * other._g;
	this->_b = this->_b * other._b;
	return *this;
}
Color3& Color3::operator/=(const Color3& other)
{
	this->_r = this->_r / other._r;
	this->_g = this->_g / other._g;
	this->_b = this->_b / other._b;
	return *this;
}
bool Color3::operator==(const Color3& other) const
{
	return (this->_r == other._r && this->_g == other._g && this->_b == other._b);
}
bool Color3::operator!=(const Color3& other) const
{
	return !(*this == other);
}

//Member Getters
unsigned char Color3::GetRed255() const
{
	return (unsigned char)this->_r * 255U;
}
unsigned char Color3::GetGreen255() const
{
	return (unsigned char)this->_g * 255U;
}
unsigned char Color3::GetBlue255() const
{
	return (unsigned char)this->_b * 255U;
}
float Color3::GetRed() const
{
	return this->_r;
}
float Color3::GetGreen() const
{
	return this->_g;
}
float Color3::GetBlue() const
{
	return this->_b;
}
float Color3::GetMagnitude() const
{
	return sqrt(pow(this->_r, 2) + pow(this->_g, 2) + pow(this->_b, 2));
}

std::string Color3::ToString() const
{
	return	std::string("(") + std::to_string(this->_r) +
			std::string(", ") + std::to_string(this->_g) +
			std::string(", ") + std::to_string(this->_b) + std::string(")");
}

//Member Setters
void Color3::SetRed255(unsigned char value)
{
	this->_r = (float)value / 255;
}
void Color3::SetGreen255(unsigned char value)
{
	this->_g = (float)value / 255;
}
void Color3::SetBlue255(unsigned char value)
{
	this->_b = (float)value / 255;
}
void Color3::SetRed(float value)
{
	this->_r = value;
}
void Color3::SetGreen(float value)
{
	this->_g = value;
}
void Color3::SetBlue(float value)
{
	this->_b = value;
}
