#ifndef _AUDIO_H
#define _AUDIO_H

#include <memory>
#include <map>
#include <irrKlang-1.5.0/irrKlang.h>

#include "../EntitySystem/System.h"

namespace Engine
{
	class AudioSource;
	class AudioSource2D;

	class Audio : public EntitySystem::System<Audio>
	{
	public:
		Audio();
		~Audio();

		virtual void initialise();
		void update();

		irrklang::ISoundSource *loadSound(const std::string &file);
		irrklang::ISound *playSound(AudioSource &source);
		irrklang::ISound *playSound2D(AudioSource2D &source);

		irrklang::ISoundEngine *getAudio();

	private:
		irrklang::ISoundEngine *audio;
		std::map<std::string, irrklang::ISoundSource *> sources;
	};
}

#endif
