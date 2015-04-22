#ifndef _AUDIOLISTENER_H
#define _AUDIOLISTENER_H

#include "../EntitySystem/Entity.h"
#include "../Core/Transform.h"

namespace Engine
{
	class AudioListener : public EntitySystem::Component<AudioListener>
	{
		friend class Audio;
	public:
		virtual void initialise();

	private:
		Transform::Handle transform;
	};
}

#endif