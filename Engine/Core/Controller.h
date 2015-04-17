#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <vector>
#include <map>

#include "Input.h"

namespace Engine
{
	class Controller : public Device<Controller>
	{
	public:
		Controller(uint32_t device);
		virtual ~Controller();

		virtual void update();
		virtual void handle(const SDL_Event &e);

		void bindButton(const SDL_GameControllerButton &gbutton, const Button &button);
		void unbindButton(const SDL_GameControllerButton &gbutton, const Button &button);

		void bindAxis(const SDL_GameControllerAxis &gaxis, const Axis &axis);
		void unbindAxis(const SDL_GameControllerAxis &gaxis, const Axis &axis);

	private:
		SDL_GameController *controller;

		// Button bindings
		std::map<SDL_GameControllerButton, std::vector<Button>> buttonBindings;

		// Axis bindings
		std::map<SDL_GameControllerAxis, std::vector<Axis>> axisBindings;
	};
}

#endif