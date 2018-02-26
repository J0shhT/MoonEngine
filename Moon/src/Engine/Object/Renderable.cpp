#include "include/Engine/Object/Renderable.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::Renderable::Renderable()
{
	DEFINE_OBJECT_CONSTRUCTOR("Renderable");
	glGenBuffers(1, &this->_vertexBufferId);
	glGenBuffers(1, &this->_colorBufferId);
	glGenBuffers(1, &this->_textureBufferId);
}

//Deconstructor
Object::Renderable::~Renderable()
{
	glDeleteBuffers(1, &this->_vertexBufferId);
	glDeleteBuffers(1, &this->_colorBufferId);
	glDeleteBuffers(1, &this->_textureBufferId);
}