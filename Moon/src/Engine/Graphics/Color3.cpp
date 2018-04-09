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
	if (r > 255)
		this->_r = 255;
	if (g > 255)
		this->_g = 255;
	if (b > 255)
		this->_b = 255;
	if (r < 0)
		this->_r = 0;
	if (g < 0)
		this->_g = 0;
	if (b < 0)
		this->_b = 0;
}
Color3::Color3(unsigned char rgb):
	_r((float)rgb/255), _g((float)rgb/255), _b((float)rgb/255)
{
	if (rgb > 255)
	{
		this->_r = 255;
		this->_g = 255;
		this->_b = 255;
	}
	if (rgb < 0)
	{
		this->_r = 0;
		this->_g = 0;
		this->_b = 0;
	}
}
Color3::Color3(float r, float g, float b): _r(r), _g(g), _b(b)
{
	if (r > 255.0f)
		this->_r = 255.0f;
	if (g > 255.0f)
		this->_g = 255.0f;
	if (b > 255.0f)
		this->_b = 255.0f;
	if (r < 0.0f)
		this->_r = 0.0f;
	if (g < 0.0f)
		this->_g = 0.0f;
	if (b < 0.0f)
		this->_b = 0.0f;
}
Color3::Color3(float rgb) : _r(rgb), _g(rgb), _b(rgb)
{
	if (rgb > 255.0f)
	{
		this->_r = 255.0f;
		this->_g = 255.0f;
		this->_b = 255.0f;
	}
	if (rgb < 0.0f)
	{
		this->_r = 0.0f;
		this->_g = 0.0f;
		this->_b = 0.0f;
	}
}

//Operators
Color3& Color3::operator=(const Color3& rhs)
{
	this->SetRed(rhs._r);
	this->SetGreen(rhs._g);
	this->SetBlue(rhs._b);
	return *this;
}
Color3& Color3::operator+=(const Color3& other)
{
	this->SetRed(this->_r + other._r);
	this->SetGreen(this->_g + other._g);
	this->SetBlue(this->_b + other._b);
	return *this;
}
Color3& Color3::operator-=(const Color3& other)
{
	this->SetRed(this->_r - other._r);
	this->SetGreen(this->_g - other._g);
	this->SetBlue(this->_b - other._b);
	return *this;
}
Color3& Color3::operator*=(const Color3& other)
{
	this->SetRed(this->_r * other._r);
	this->SetGreen(this->_g * other._g);
	this->SetBlue(this->_b * other._b);
	return *this;
}
Color3& Color3::operator/=(const Color3& other)
{
	this->SetRed(this->_r / other._r);
	this->SetGreen(this->_g / other._g);
	this->SetBlue(this->_b / other._b);
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

std::string Color3::ToString() const
{
	return	std::string("(") + std::to_string(this->_r) +
			std::string(", ") + std::to_string(this->_g) +
			std::string(", ") + std::to_string(this->_b) + std::string(")");
}

//Member Setters
void Color3::SetRed255(unsigned char value)
{
	if (value > 255)
		value = 255;
	if (value < 0)
		value = 0;
	this->_r = (float)value / 255;
}
void Color3::SetGreen255(unsigned char value)
{
	if (value > 255)
		value = 255;
	if (value < 0)
		value = 0;
	this->_g = (float)value / 255;
}
void Color3::SetBlue255(unsigned char value)
{
	if (value > 255)
		value = 255;
	if (value < 0)
		value = 0;
	this->_b = (float)value / 255;
}
void Color3::SetRed(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	if (value < 0.0f)
		value = 0.0f;
	this->_r = value;
}
void Color3::SetGreen(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	if (value < 0.0f)
		value = 0.0f;
	this->_g = value;
}
void Color3::SetBlue(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	if (value < 0.0f)
		value = 0.0f;
	this->_b = value;
}
