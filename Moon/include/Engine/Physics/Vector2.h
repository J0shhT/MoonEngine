#pragma once

#include "include/Common.h"

#include <boost/math/constants/constants.hpp>

namespace Moon::Physics
{

	class Vector2
	{

		public:
			//Constructors
			Vector2();
			Vector2(double x, double y);
			Vector2(double xy);

			//Destructor
			~Vector2() noexcept = default;

			//Operators
			Vector2& operator=(const Vector2&);
			Vector2& operator+=(const Vector2&);
			Vector2& operator-=(const Vector2&);
			Vector2& operator*=(const Vector2&);
			Vector2& operator/=(const Vector2&);
			bool operator==(const Vector2&) const;
			bool operator!=(const Vector2&) const;
			bool operator>(const Vector2&) const;
			bool operator>=(const Vector2&) const;
			bool operator<(const Vector2&) const;
			bool operator<=(const Vector2&) const;

			//Getters
			double GetX() const;
			double GetY() const;
			double GetMagnitude() const;
			double GetArea() const;
			double GetAngle(bool convertToDegrees = false) const;
			std::string ToString() const;

			//Setters
			void SetX(double);
			void SetY(double);

		private:
			double x;
			double y;

	};

	struct GLCoordinates2f {
		float x;
		float y;
	};

	GLCoordinates2f ConvertToGLCoordinates(const Moon::Physics::Vector2&);

}