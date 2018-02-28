#pragma once

#include "include/Common.h"

#include "Renderable.h"
#include "include/Engine/Physics/Vector2.h"
#include "include/Engine/Graphics/Color3.h"

#include "include/Engine/ContentProvider.h"

//#define DEFINE_DEFAULT_PHYSICS_STEPPER()

namespace Moon::Object {

	class PVObject : public Renderable {

	public:
		PVObject();
		virtual ~PVObject() noexcept;

		//Member Getters
		Moon::Physics::Vector2 GetPosition() const;
		Moon::Physics::Vector2 GetSize() const;
		Moon::Physics::Vector2 GetVelocity() const;
		bool IsAnchored() const;
		bool IsSolid() const;
		Moon::Graphics::Color3 GetColor() const;
		std::string GetTextureContentId() const;

		//Member Setters
		void SetPosition(Moon::Physics::Vector2);
		void SetSize(Moon::Physics::Vector2);
		void SetVelocity(Moon::Physics::Vector2);
		void SetAnchored(bool);
		void SetSolid(bool);
		void SetColor(Moon::Graphics::Color3);
		bool SetTexture(std::string filePath);

		//Methods
		void CheckCollisions(std::vector<std::shared_ptr<PVObject>> physicsObjects);
		void StepPhysics(double frameDeltaSec);

	protected:
		//Members
		Moon::Physics::Vector2 _position;
		Moon::Physics::Vector2 _size;
		Moon::Physics::Vector2 _velocity;
		bool _isAnchored;
		bool _isSolid;
		Moon::Graphics::Color3 _color;
		ContentId _textureContentId;

		//Internal
		bool _hasCollisionTop;
		bool _hasCollisionBottom;
		bool _hasCollisionLeft;
		bool _hasCollisionRight;

	};

}