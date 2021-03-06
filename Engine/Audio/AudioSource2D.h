#ifndef _AudioSource2D_H
#define _AudioSource2D_H

#include <irrKlang-1.5.0/irrKlang.h>
#include <string>

#include "../EntitySystem/Entity.h"
#include "../Core/Transform.h"
#include "Audio.h"

namespace Engine
{
	class AudioSource2D : public EntitySystem::Component<AudioSource2D>, public irrklang::ISoundStopEventReceiver
	{
		friend class Audio;
	public:
		AudioSource2D(const std::string &file, unsigned int volume = 100, bool loop = false, bool playOnAwake = false);

		// Required components / component setup
		virtual void initialise();
		virtual void uninitialise();

		void play(unsigned int delay = 0);
		void pause();
		void unpause();
		void stop();

		virtual void OnSoundStopped(irrklang::ISound *sound, irrklang::E_STOP_EVENT_CAUSE reason, void *userData);

	private:
		std::string file;
		bool playOnAwake;
		bool loop;
		int loops;
		unsigned int volume;

		// Audio system
		Audio::Handle audio;

		// Sound clip to play
		irrklang::ISoundSource *clip = nullptr;

		// State
		irrklang::ISound *sound = nullptr;
		bool playing;
		bool paused;

	};
}

#endif
