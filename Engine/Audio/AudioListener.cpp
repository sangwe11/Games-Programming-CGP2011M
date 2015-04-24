#include "AudioListener.h"

namespace Engine
{
	void AudioListener::initialise()
	{
		// Require transform component
		transform = entity.getComponent<Transform>();
	}
}