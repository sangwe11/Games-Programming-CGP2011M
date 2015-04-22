#include "Time.h"

#include <SDL/SDL.h>

namespace Engine
{
	void Time::initialise()
	{
		addUpdateFunction(&Time::update, *this, 1000);
	}

	void Time::update()
	{
		// Update delta time
		unsigned int currentTime = SDL_GetTicks();
		unsigned int frameTime = currentTime - lastTime;
		lastTime = currentTime;
		deltaTime = frameTime / 1000.0f;
	}

	float Time::getDeltaTime()
	{
		return deltaTime;
	}
}