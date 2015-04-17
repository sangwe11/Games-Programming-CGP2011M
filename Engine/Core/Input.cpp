#include "Input.h"

#include "../EntitySystem/World.h"

namespace Engine
{
	void Input::initialise()
	{
		// Handle initial events
	}

	void Input::uninitialise()
	{
		// Clear device list
		devices.clear();
	}

	void Input::update(EntitySystem::EntityManager &entities)
	{
		SDL_Event e;

		// Update each device
		for (auto &list : devices)
			for (auto &device : list.second)
				device.second->update();

		// Poll for SDL events
		while (SDL_PollEvent(&e))
		{
			// Handle quit event
			if (e.type == SDL_QUIT)
			{
				// Set quit flag
				quit = true;
			}
			// Handle mouse motion
			else if (e.type == SDL_MOUSEMOTION)
			{
				//if (exists<KeyboardMouse>(e.motion.which))
				//	get<KeyboardMouse>(e.motion.which).handle(e);
			}
			// Handle mouse buttons
			else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONDOWN)
			{
				//if (exists<KeyboardMouse>(e.button.which))
				//	get<KeyboardMouse>(e.button.which).handle(e);
			}
		}
	}
}