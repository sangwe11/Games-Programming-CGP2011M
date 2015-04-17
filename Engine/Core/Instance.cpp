#include "Instance.h"

#include <iostream>
#include <SDL/SDL.h>

namespace Engine
{
	Instance::Instance()
	{
		setup();
	}

	Instance::~Instance()
	{
		cleanup();
	}

	void Instance::setup()
	{
		// Initialise SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			// Error and exit with error code 1
			std::cout << "SDL initialisation failed!" << std::endl;
			exit(1);
		}
	}

	void Instance::cleanup()
	{
		// Quit SDL
		SDL_Quit();
	}
}