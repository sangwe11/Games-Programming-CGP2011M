#include "Input.h"

#include "../EntitySystem/World.h"
#include "KeyboardMouse.h"
#include "Controller.h"

namespace Engine
{
	const bool BaseDevice::getButton(const Button &button)
	{
		return buttons[button];
	}

	const bool BaseDevice::getButtonDown(const Button &button)
	{
		return (buttons[button] && !buttonsOld[button]);
	}

	const bool BaseDevice::getButtonUp(const Button &button)
	{
		return (!buttons[button] && buttonsOld[button]);
	}

	const float BaseDevice::getAxis(const Axis &axis)
	{
		return axes[axis];
	}

	void Input::initialise()
	{
		// Add SDL keyboard/mouse device
		devices[KeyboardMouse::getTypeId()][0] = std::make_unique<KeyboardMouse>();

		// Add function to update loop
		addUpdateFunction(&Input::update, *this, 10, false);

		// Handle initial events
		update();
	}

	void Input::uninitialise()
	{
		// Clear device list
		devices.clear();
	}

	void Input::update()
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
				if (exists<KeyboardMouse>(e.motion.which))
					get<KeyboardMouse>(e.motion.which).handle(e);
			}
			// Handle mouse buttons
			else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (exists<KeyboardMouse>(e.button.which))
					get<KeyboardMouse>(e.button.which).handle(e);
			}
			// Handle controller added
			else if (e.type == SDL_CONTROLLERDEVICEADDED)
			{
				// Get controller typeid
				EntitySystem::TypeId typeId = Controller::getTypeId();

				// Create new controller
				devices[typeId][e.cdevice.which] = std::make_unique<Controller>(e.cdevice.which);

				// Map device to active device
				controllerDevices[devices[typeId][e.cdevice.which]->getId()] = e.cdevice.which;

				// Print debug message
				std::cout << "Controller " << e.cdevice.which << " added." << std::endl;
			}
			// Handle controller removed
			else if (e.type == SDL_CONTROLLERDEVICEREMOVED)
			{
				// Remove the device
				devices[Controller::getTypeId()].erase(controllerDevices[e.cdevice.which]);

				// Unmap from active devices
				controllerDevices.erase(e.cdevice.which);

				// Print debug message
				std::cout << "Controller " << controllerDevices[e.cdevice.which] << " removed." << std::endl;
			}
			// Handle controller input
			else if (e.type == SDL_CONTROLLERAXISMOTION || e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP)
			{
				if (exists<Controller>(e.cdevice.which))
					get<Controller>(e.cdevice.which).handle(e);
			}
		}
	}
}