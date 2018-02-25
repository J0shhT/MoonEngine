#pragma once

#include "include/Common.h"

#include "Object.h"

#include "include/Engine/Object/PVObject.h"
#include "include/Engine/Physics/Vector2.h"

#include <SDL/SDL.h>

namespace Moon::Enum {

	enum CameraType {
		Scriptable,
		FreeCamera,
		Attach,
		Follow
	};

}

namespace Moon::Object {

	class Camera : public Object {

		public:
			Camera();
			virtual ~Camera() noexcept;

			//Member Getters
			Enum::CameraType GetCameraType() const;
			Physics::Vector2 GetPosition() const;
			double GetMovementSpeed() const;
			Physics::Vector2 GetMovementDirection() const;
			char GetMovementDirectionX() const;
			char GetMovementDirectionY() const;
			std::shared_ptr<PVObject> GetTarget() const;

			//Member Setters
			void SetCameraType(Enum::CameraType);
			void SetPosition(Physics::Vector2);
			void SetMovementSpeed(double);
			void SetMovementDirection(char x, char y);
			void SetMovementDirectionX(char);
			void SetMovementDirectionY(char);
			void SetTarget(std::shared_ptr<PVObject>);

			//Methods
			void Update(double frameDeltaSec);
			Physics::Vector2 GetVelocity() const;

			//Event functions
			///These are called by GameHandler::ProcessEvents()
			void KeyDownEvent(SDL_Keycode);
			void KeyUpEvent(SDL_Keycode);

		protected:
			//Members
			Enum::CameraType _cameraType;
			Physics::Vector2 _position;
			double _speed;
			char _movementDirectionX;
			char _movementDirectionY;
			std::weak_ptr<PVObject> _target;

	};

}