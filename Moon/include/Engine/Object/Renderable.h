#pragma once

#include "include/Common.h"

#include "Object.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Moon::Object {

	class Renderable : public Object {

	public:
		Renderable();
		virtual ~Renderable() noexcept;

		//Member Getters


		//Member Setters


		//Methods
		virtual void Render(GLuint shaderId, GLuint matrixId) = 0;

	protected:
		//Members
		GLuint _vertexBufferId;
		GLuint _colorBufferId;
		GLuint _textureBufferId;

	};

}