#include "include/Engine/SoundService.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"
#include "include/Engine/ContentProvider.h"

using namespace Moon;


//Constructor
SoundService::SoundService()
{
	assert(SoundService::instance == nullptr); //Make sure we aren't making 2 instances (SoundService is a singleton)...
	SoundService::instance = this;
}

//Deconstructor
SoundService::~SoundService()
{
	
}

//Singleton Getter
SoundService* SoundService::instance = nullptr;
SoundService* SoundService::singleton()
{
	assert(SoundService::instance != nullptr); //Make sure we never call :singleton() before it's made...
	return SoundService::instance;
}

//Methods
void SoundService::PlaySound(std::string filePath, float volume)
{
	ContentId contentId = ContentProvider::singleton()->LoadFileSound(filePath);
	if (contentId != "")
	{
		Content soundContent = ContentProvider::singleton()->Get(contentId);
		if (soundContent.type == ContentType::FMODFileSound)
		{
			FMOD::System* fmod = GameHandler::singleton()->GetSoundSystem();
			fmod->playSound(soundContent.fmod_Sound, 0, false, &soundContent.fmod_Channel);
			soundContent.fmod_Channel->setVolume(volume);
		}
	}
}
