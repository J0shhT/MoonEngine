#pragma once

#include "include/Common.h"

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/glut.h>

namespace Moon::Graphics
{

	class FileTexture {

		public:
			FileTexture();
			~FileTexture();

			//Member Getters
			bool IsLoaded() const;
			GLuint GetTextureId() const;
			std::string GetFilePath() const;
			unsigned char* GetImage() const;
			int GetWidth() const;
			int GetHeight() const;

			//Methods
			bool Load(std::string filePath);
			void Delete();

		private:
			//Members
			bool _isLoaded;
			GLuint _textureId;
			std::string _filePath;
			unsigned char* _image;
			int _width;
			int _height;

	};

}