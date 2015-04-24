#include "Audio.h"
#include "AudioListener.h"
#include "../EntitySystem/World.h"
#include "AudioSource.h"
#include "AudioSource2D.h"

#include <SDL/SDL.h>

#include <iostream>

namespace Engine
{
	Audio::Audio()
	{
		// Start irrklang engine with default params
		audio = irrklang::createIrrKlangDevice();

		// Check for errors
		if (audio == nullptr)
		{
			std::cout << "Error creating irrklang sound engine." << std::endl;
			exit(1);
		}

		// Grab
		audio->grab();
	}

	Audio::~Audio()
	{
		// Drop the irrklang engine
		audio->drop();
	}

	void Audio::initialise()
	{
		// Add update function to main
		addUpdateFunction(&Audio::update, *this, 35);
	}

	irrklang::ISoundSource *Audio::loadSound(const std::string &file)
	{
		// Check if sound source already exists
		if (sources[file] != nullptr)
		{
			// Return already loaded sound
			return sources[file];
		}
		else
		{
			// Load the sound source
			irrklang::ISoundSource *source = audio->addSoundSourceFromFile(file.c_str());

			// Check for error
			if (source == nullptr)
			{
				std::cout << "Error loading sound file: " << file << std::endl;
				return nullptr;
			}

			// Add to sources
			sources[file] = source;
		}

		// Return the source
		return sources[file];
	}

	irrklang::ISound *Audio::playSound(AudioSource &source)
	{
		irrklang::ISound *sound = audio->play3D(source.clip, irrklang::vec3df(source.transform->position.x, source.transform->position.y, source.transform->position.z), source.loop, true);

		if (sound == nullptr)
		{
			std::cout << "Couldn't play sound clip: " << source.file << std::endl;
		}
		else
		{
			// Set variables
			sound->setSoundStopEventReceiver(&source, nullptr);
			sound->setVolume(source.volume / 100.0f);

			// Unpause
			sound->setIsPaused(false);
		}

		return sound;
	}

	irrklang::ISound *Audio::playSound2D(AudioSource2D &source)
	{
		irrklang::ISound *sound = audio->play2D(source.clip, source.loop, false, true);

		if (sound == nullptr)
		{
			std::cout << "Couldn't play sound clip: " << source.file << std::endl;
		}
		else
		{
			// Set variables
			sound->setSoundStopEventReceiver(&source, nullptr);
			sound->setVolume(source.volume / 100.0f);

			// Unpause
			sound->setIsPaused(false);
		}

		return sound;
	}

	void Audio::update()
	{
		// Loop through all audio source components and update positions
		for (AudioSource::Handle &source : manager->getWorld().entities.getAllComponents<AudioSource>(true))
		{
			if (source->playing)
			{
				if (source->sound == nullptr)
				{
					std::cout << "Sound missing: " << source->file << std::endl;
					continue;
				}

				// Sound position
				glm::vec3 worldPosition = source->transform->positionWorld();

				// Convert to irrklangs vector type
				irrklang::vec3df position(worldPosition.x, worldPosition.y, worldPosition.z);

				// Set position
				source->sound->setPosition(position);

				// For audio sources attached to entities with a rigidbody, update the velocity too
				//if (source.body != nullptr)
				//{
				//	irrklang::vec3df velocity(source->body->velocity.x, source->body->velocity.y, source->body->velocity.z);

				//	// Set velocity
				//	source->sound->setVelocity(velocity);
				//}
			}
		}

		// Grab the audio listener - grab all of them to error if there is more than one
		std::vector<AudioListener::Handle> listeners = manager->getWorld().entities.getAllComponents<AudioListener>(true);

		// There should only ever be one audio listener
		if (listeners.size() <= 0)
		{
			std::cout << "Error: No AudioListener!" << std::endl;
			return;
		}
		else if (listeners.size() > 1)
		{
			std::cout << "Error: There should only ever be 1 AudioListener!" << std::endl;
		}

		// Update position of audio listener
		irrklang::vec3df listenerPosition(listeners[0]->transform->position.x, listeners[0]->transform->position.y, listeners[0]->transform->position.z);
		irrklang::vec3df listenerForward(-listeners[0]->transform->forward().x, -listeners[0]->transform->forward().y, -listeners[0]->transform->forward().z);
		audio->setListenerPosition(listenerPosition, listenerForward);
	}


	irrklang::ISoundEngine *Audio::getAudio()
	{
		return audio;
	}
}