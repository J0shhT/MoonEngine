#pragma once

#include "include/Common.h"

#include "PVObject.h"

namespace Moon::Object {

	class Rectangle : public PVObject {

	public:
		Rectangle();
		virtual ~Rectangle() noexcept;

		//Member Getters
		Moon::Physics::Vector2 GetSize() const;

		//Member Setters
		void SetSize(Moon::Physics::Vector2);

		//Methods
		void Render(GLuint matrixId);
		void StepPhysics(double frameDeltaSec);

	protected:
		//Members
		Moon::Physics::Vector2 _size;

	};

}