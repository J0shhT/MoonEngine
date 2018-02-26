#include "include/Engine/Graphics/FileTexture.h"

using namespace Moon::Graphics;

//Constructor
FileTexture::FileTexture(): _isLoaded(false)
{
	glGenTextures(1, &this->_textureId);
}

//Deconstructor
FileTexture::~FileTexture()
{
	glDeleteTextures(1, &this->_textureId);
}

//Member Getters
bool FileTexture::IsLoaded() const
{
	return this->_isLoaded;
}
GLuint FileTexture::GetTextureId() const
{
	return this->_textureId;
}
std::string FileTexture::GetFilePath() const
{
	return this->_filePath;
}
unsigned char* FileTexture::GetImage() const
{
	return this->_image;
}
int FileTexture::GetWidth() const
{
	return this->_width;
}
int FileTexture::GetHeight() const
{
	return this->_height;
}

//Methods
bool FileTexture::Load(std::string filePath)
{
	#ifdef _DEBUG
		filePath = DEBUG_DIR + filePath;
	#endif
	this->_image = SOIL_load_image(filePath.c_str(), &this->_width, &this->_height, 0, SOIL_LOAD_RGBA);
	glBindTexture(GL_TEXTURE_2D, this->_textureId);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		this->_width,
		this->_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->_image
	);
	bool didLoad = this->_image;
	SOIL_free_image_data(this->_image);
	this->_isLoaded = didLoad;
	return didLoad;
}
void FileTexture::Delete()
{
	this->_isLoaded = false;
}
