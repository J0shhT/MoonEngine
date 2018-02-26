#pragma once

#include "include/Common.h"

namespace Moon::Graphics
{

	class Color3
	{

		public:
			//Constructors
			Color3();
			Color3(unsigned char r, unsigned char g, unsigned char b);
			Color3(unsigned char rgb);
			Color3(float r, float g, float b);
			Color3(float rgb);

			//Destructor
			~Color3() noexcept = default;

			//Operators
			Color3& operator=(const Color3&);
			Color3& operator+=(const Color3&);
			Color3& operator-=(const Color3&);
			Color3& operator*=(const Color3&);
			Color3& operator/=(const Color3&);
			bool operator==(const Color3&) const;
			bool operator!=(const Color3&) const;

			//Getters
			unsigned char GetRed255() const;
			unsigned char GetGreen255() const;
			unsigned char GetBlue255() const;
			float GetRed() const;
			float GetGreen() const;
			float GetBlue() const;
			float GetMagnitude() const;
			std::string ToString() const;

			//Setters
			void SetRed255(unsigned char);
			void SetGreen255(unsigned char);
			void SetBlue255(unsigned char);
			void SetRed(float);
			void SetGreen(float);
			void SetBlue(float);

		private:
			float _r;
			float _g;
			float _b;

	};

}