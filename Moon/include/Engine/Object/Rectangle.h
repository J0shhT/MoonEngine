#pragma once

#include "include/Common.h"

#include "PVObject.h"

namespace Moon::Object {

	class Rectangle : public PVObject {

	public:
		Rectangle();
		virtual ~Rectangle() noexcept;

		//Member Getters
		

		//Member Setters
		

		//Methods
		void Render(GLuint shaderId, GLuint matrixId);

	protected:
		//Members
		

	};

}