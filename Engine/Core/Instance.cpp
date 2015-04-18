#include "Instance.h"

#include <iostream>
#include <SDL/SDL.h>

#include "Files.h"
#include "Input.h"

namespace Engine
{
	Instance::Instance()
	{
		// Initialise SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			// Error and exit with error code 1
			std::cout << "SDL initialisation failed!" << std::endl;
			exit(1);
		}
	}

	Instance::~Instance()
	{
		// Quit SDL
		SDL_Quit();
	}

	void Instance::setup()
	{
		// Add systems
		systems.addSystem<Files>();
		systems.addSystem<Input>();

		// Check all systems exist
		if (!systems.systemExists<Files>() || !systems.systemExists<Input>())
		{
			std::cout << "Error creating subsystems." << std::endl;
			cleanup();
			exit(1);
		}
	}

	void Instance::cleanup()
	{
		// Reset entity world
		entities.reset();
	}

	void Instance::update()
	{
		// Update all systems
		systems.update();
	}
}