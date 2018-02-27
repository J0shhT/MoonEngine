#pragma once

#include "include/Common.h"

#include "Object.h"

#include "include/Engine/Object/PVObject.h"

#include <SDL/SDL.h>

namespace Moon::Object {

	class Player : public Object {

		public:
			Player();
			virtual ~Player();

			//Member Getters
			std::shared_ptr<PVObject> GetCharacter() const;
			double GetWalkSpeed() const;
			bool IsControllable() const;

			//Member Setters
			void SetWalkSpeed(double);
			void SetIsControllable(bool);

			//Methods
			void LoadCharacter();
			
			//Event functions
			///These are called by GameHandler::ProcessEvents()
			void KeyDownEvent(SDL_Keycode);
			void KeyUpEvent(SDL_Keycode);

		protected:
			//Members
			std::weak_ptr<PVObject> _character;
			double _walkSpeed;
			char _movementDirectionX;
			char _movementDirectionY;
			bool _isControllable;

			//Methods
			void _updateCharacterVelocity();

	};

}