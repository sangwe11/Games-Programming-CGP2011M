#include "AudioSource.h"
#include "Audio.h"
#include "../EntitySystem/World.h"

#include <GLM/glm.hpp>

namespace Engine
{
	AudioSource::AudioSource(const std::string &file, unsigned int volume, bool loop, bool playOnAwake)
	{
		this->file = file;
		this->playOnAwake = playOnAwake;
		this->loop = loop;
		this->loops = loops;
		this->playing = false;
		this->paused = false;

		// Clamp volume between 0 and 100%
		this->volume = glm::clamp(volume, (unsigned int)0, (unsigned int)100);
	}

	AudioSource::~AudioSource()
	{
		// Remove reference to clip
		clip = nullptr;
	}

	void AudioSource::initialise()
	{
		// Require transform component
		transform = entity.getComponent<Transform>();

		// Grab the audio system
		audio = manager->getWorld().systems.getSystem<Audio>();

		// Load the audio file
		clip = audio->loadSound(file);

		if (clip == nullptr || clip == 0)
		{
			std::cout << "Failed to load sound: " << file << std::endl;
			return;
		}

		// Grab
		clip->grab();

		// Play on awake?
		if (playOnAwake)
			play();
	}

	void AudioSource::play(unsigned int delay)
	{
		if (playing && paused)
		{
			// Just unpaused the sound
			sound->setIsPaused(false);
		}
		else if (!playing)
		{
			// Play the sound - start paused
			if (clip != nullptr)
			{
				playing = true;
				sound = audio->playSound(*this);

				if (sound != nullptr)
					sound->grab();
			}
			else
			{
				std::cout << "Can't play sound as file isn't loaded: " << file << std::endl;
			}
		}
	}

	void AudioSource::pause()
	{
		if (playing && !paused)
		{
			sound->setIsPaused(true);
			paused = true;
		}
	}

	void AudioSource::unpause()
	{
		if (playing && paused)
		{
			sound->setIsPaused(false);
			paused = false;
		}
	}

	void AudioSource::stop()
	{
		if (playing)
		{
			sound->stop();
		}
	}

	void AudioSource::OnSoundStopped(irrklang::ISound *sound, irrklang::E_STOP_EVENT_CAUSE reason, void *userData)
	{
		// No longer playing
		this->paused = false;
		this->playing = false;

		// Drop the sound
		sound->drop();
	}
}