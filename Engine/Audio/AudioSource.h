#ifndef _AUDIOSOURCE_H
#define _AUDIOSOURCE_H

#include <irrKlang-1.5.0/irrKlang.h>
#include <string>

#include "../EntitySystem/Entity.h"
#include "../Core/Transform.h"
#include "Audio.h"

namespace Engine
{
	class AudioSource : public EntitySystem::Component<AudioSource>, public irrklang::ISoundStopEventReceiver
	{
		friend class Audio;
	public:
		AudioSource(const std::string &file, unsigned int volume = 100, bool loop = false, bool playOnAwake = false);
		virtual ~AudioSource();

		// Required components / component setup
		virtual void initialise();

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

		// Component handles
		Transform::Handle transform;

		// Sound clip to play
		irrklang::ISoundSource *clip;

		// State
		irrklang::ISound *sound;
		bool playing;
		bool paused;

	};
}

#endif
