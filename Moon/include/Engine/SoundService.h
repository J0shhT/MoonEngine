#pragma once

#include "include/Common.h"

#include <fmod/fmod.hpp>

#define MMNOSOUND // Why does Windows API have PlaySound as a macro?!?!

namespace Moon {

	class SoundService {

		public:
			SoundService();
			~SoundService() noexcept;

			//Singleton Getter
			static SoundService* singleton();

			//Methods
			void PlaySound(std::string filePath, float volume = 1.0f);

		private:
			//Members
			static SoundService* instance;

	};

}