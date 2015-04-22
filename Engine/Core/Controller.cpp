#include "Controller.h"

#include <algorithm>

namespace Engine
{
	Controller::Controller(uint32_t device)
	{
		controller = SDL_GameControllerOpen(device);
		id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
		name = SDL_GameControllerName(controller);

		// Default button mapping
		bindButton(SDL_CONTROLLER_BUTTON_A, "jump");
		bindButton(SDL_CONTROLLER_BUTTON_B, "back");
		bindButton(SDL_CONTROLLER_BUTTON_X, "reload");
		bindButton(SDL_CONTROLLER_BUTTON_Y, "inventory");

		bindButton(SDL_CONTROLLER_BUTTON_START, "start");
		bindButton(SDL_CONTROLLER_BUTTON_BACK, "quit");

		bindButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, "special1");
		bindButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, "special2");

		bindButton(SDL_CONTROLLER_BUTTON_LEFTSTICK, "sprint");
		bindButton(SDL_CONTROLLER_BUTTON_RIGHTSTICK, "crouch");

		// Default axis mapping
		bindAxis(SDL_CONTROLLER_AXIS_LEFTX, "moveX");
		bindAxis(SDL_CONTROLLER_AXIS_LEFTY, "moveY");
		bindAxis(SDL_CONTROLLER_AXIS_RIGHTX, "lookX");
		bindAxis(SDL_CONTROLLER_AXIS_RIGHTY, "lookY");
		bindAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, "aim");
		bindAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, "fire");
	}

	Controller::~Controller()
	{
		SDL_GameControllerClose(controller);
	}

	void Controller::update()
	{
		// Copy button state to old button state
		buttonsOld = buttons;
	}

	void Controller::handle(const SDL_Event &e)
	{
		// Handle the event
		switch (e.type)
		{
			// Handle controller axis motion
		case SDL_CONTROLLERAXISMOTION:
		{
										 // Cast back to SDL type
										 SDL_GameControllerAxis gaxis = static_cast<SDL_GameControllerAxis>(e.caxis.axis);

										 // "Dead zone" is between -3200 and 3200
										 if (e.caxis.value > 8000 || e.caxis.value < -8000)
										 {
											 // Set normalised value
											 for (const Axis &axis : axisBindings[gaxis])
												 axes[axis] = e.caxis.value / 32767.0f;
										 }
										 else
										 {
											 // At rest in if in "dead zone"
											 for (const Axis &axis : axisBindings[gaxis])
												 axes[axis] = 0.0f;
										 }
		}
			break;

			// Handle controller button press
		case SDL_CONTROLLERBUTTONDOWN:
		{
										 // Cast back to SDL type
										 SDL_GameControllerButton gbutton = static_cast<SDL_GameControllerButton>(e.cbutton.button);

										 // Set any buttons binded to controller button
										 for (const Button &button : buttonBindings[gbutton])
											 buttons[button] = true;
		}
			break;

			// Handle controller button release
		case SDL_CONTROLLERBUTTONUP:
		{
									   // Cast back to SDL type
									   SDL_GameControllerButton gbutton = static_cast<SDL_GameControllerButton>(e.cbutton.button);

									   // Unset any buttons binded to controller button
									   for (const Button &button : buttonBindings[gbutton])
										   buttons[button] = false;
		}
			break;
		}
	}

	void Controller::bindButton(const SDL_GameControllerButton &gbutton, const Button &button)
	{
		buttonBindings[gbutton].push_back(button);
	}

	void Controller::unbindButton(const SDL_GameControllerButton &gbutton, const Button &button)
	{
		buttonBindings[gbutton].erase(std::remove(buttonBindings[gbutton].begin(), buttonBindings[gbutton].end(), button), buttonBindings[gbutton].end());
	}

	void Controller::bindAxis(const SDL_GameControllerAxis &gaxis, const Axis &axis)
	{
		axisBindings[gaxis].push_back(axis);
	}

	void Controller::unbindAxis(const SDL_GameControllerAxis &gaxis, const Axis &axis)
	{
		axisBindings[gaxis].erase(std::remove(axisBindings[gaxis].begin(), axisBindings[gaxis].end(), axis), axisBindings[gaxis].end());
	}
}