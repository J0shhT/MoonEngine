#pragma once

#include "include/Common.h"

#include "Renderable.h"
#include "include/Engine/Physics/Vector2.h"
//#include "include/Engine/Graphics/Color3.h"

//#define DEFINE_DEFAULT_PHYSICS_STEPPER()

namespace Moon::Object {

	class PVObject : public Renderable {

	public:
		PVObject();
		virtual ~PVObject() noexcept;

		//Member Getters
		Moon::Physics::Vector2 GetPosition() const;
		double GetRotation() const;
		Moon::Physics::Vector2 GetVelocity() const;
		double GetRotVelocity() const;
		bool IsAnchored() const;
		bool IsSolid() const;
		//Moon::Graphics::Color3 GetColor() const;

		//Member Setters
		void SetPosition(Moon::Physics::Vector2);
		void SetRotation(double);
		void SetVelocity(Moon::Physics::Vector2);
		void SetRotVelocity(double);
		void SetAnchored(bool);
		void SetSolid(bool);
		//void SetColor(Moon::Graphics::Color3);

		//Methods
		virtual void StepPhysics(double frameDeltaSec) = 0;

	protected:
		//Members
		Moon::Physics::Vector2 _position;
		double _rotation;
		Moon::Physics::Vector2 _velocity;
		double _rotVelocity;
		bool _isAnchored;
		bool _isSolid;
		//Moon::Graphics::Color3 _color;

	};

}