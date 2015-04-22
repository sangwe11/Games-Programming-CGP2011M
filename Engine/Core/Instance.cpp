#include "Instance.h"

#include <iostream>
#include <SDL/SDL.h>

#include "Files.h"
#include "Input.h"
#include "../Rendering/Display.h"
#include "../Rendering/Rendering.h"

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

	void Instance::setup(const std::string &title, const unsigned int &width, const unsigned int &height, const bool &fullscreen, const bool &vsync)
	{
		// Add systems
		systems.addSystem<Files>();
		systems.addSystem<Input>();
		systems.addSystem<Display>(title, width, height, fullscreen, vsync);
		systems.addSystem<Rendering>();

		// Check all systems exist
		if (!systems.systemExists<Files>() || !systems.systemExists<Input>() || !systems.systemExists<Display>())
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

		// Remove all systems
		systems.removeAll();
	}

	void Instance::update()
	{
		// Update all systems
		systems.update();
	}
}