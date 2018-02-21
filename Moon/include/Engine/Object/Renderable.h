#pragma once

#include "include/Common.h"

#include "Object.h"

#include <GL/glew.h>
#include <GL/glut.h>

namespace Moon::Object {

	class Renderable : public Object {

	public:
		Renderable();
		virtual ~Renderable() noexcept;

		//Member Getters


		//Member Setters


		//Methods
		virtual void Render() = 0;

	protected:
		//Members


	};

}