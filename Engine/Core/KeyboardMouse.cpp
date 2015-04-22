#include "KeyboardMouse.h"

#include <algorithm>

namespace Engine
{
	KeyboardMouse::KeyboardMouse()
	{
		id = 0;
		name = "KeyboardMouse0";
		keystate = SDL_GetKeyboardState(NULL);

		// Default button mapping
		bindKeyboardButton(SDL_SCANCODE_SPACE, "jump");
		bindKeyboardButton(SDL_SCANCODE_BACKSPACE, "back");
		bindKeyboardButton(SDL_SCANCODE_R, "reload");
		bindKeyboardButton(SDL_SCANCODE_I, "inventory");

		bindKeyboardButton(SDL_SCANCODE_RETURN, "start");
		bindKeyboardButton(SDL_SCANCODE_ESCAPE, "quit");

		bindKeyboardButton(SDL_SCANCODE_1, "special1");
		bindKeyboardButton(SDL_SCANCODE_1, "special2");

		bindKeyboardButton(SDL_SCANCODE_LSHIFT, "sprint");
		bindKeyboardButton(SDL_SCANCODE_RSHIFT, "sprint");
		bindKeyboardButton(SDL_SCANCODE_LCTRL, "crouch");
		bindKeyboardButton(SDL_SCANCODE_RCTRL, "crouch");

		bindMouseButton(SDL_BUTTON_LEFT, "fire");
		bindMouseButton(SDL_BUTTON_RIGHT, "aim");

		// Default axis mapping
		bindKeyboardAxis(SDL_SCANCODE_S, SDL_SCANCODE_W, "moveY");
		bindKeyboardAxis(SDL_SCANCODE_D, SDL_SCANCODE_A, "moveX");
		bindMouseAxes("lookX", "lookY");
	}

	void KeyboardMouse::update()
	{
		// Copy button state to old button state
		buttonsOld = buttons;

		// Update button states
		for (std::pair<SDL_Scancode, std::vector<Button>> binding : buttonBindings)
			for (const Button &button : binding.second)
				buttons[button] = keystate[binding.first] != 0;

		// Update axes
		for (std::pair<std::pair<SDL_Scancode, SDL_Scancode>, std::vector<Axis>> binding : axisBindings)
		{
			float value = 0.0f;

			if (keystate[binding.first.first])
				value += 1.0f;

			if (keystate[binding.first.second])
				value -= 1.0f;

			for (const Axis &axis : binding.second)
				axes[axis] = value;
		}
	}

	void KeyboardMouse::handle(const SDL_Event &e)
	{
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		{
									// Set any buttons binded to controller button
									for (Button button : mouseButtonBindings[e.button.button])
										buttons[button] = true;
		}
			break;

		case SDL_MOUSEBUTTONUP:
		{
								  // Set any buttons binded to controller button
								  for (Button button : mouseButtonBindings[e.button.button])
									  buttons[button] = false;
		}
			break;

		case SDL_MOUSEMOTION:
		{
								float xvalue = e.motion.xrel * 0.05f;
								float yvalue = e.motion.yrel * 0.05f;

								for (std::pair<Axis, Axis> pair : mouseAxisBindings)
								{
									if (e.motion.xrel > 1 || e.motion.xrel < -1)
										axes[pair.first] = xvalue;
									else
										axes[pair.first] = 0.0f;

									if (e.motion.yrel > 1 || e.motion.yrel < -1)
										axes[pair.second] = yvalue;
									else
										axes[pair.second] = 0.0f;
								}
		}
			break;
		}
	}

	void KeyboardMouse::bindKeyboardButton(const SDL_Scancode &key, const Button &button)
	{
		buttonBindings[key].push_back(button);
	}

	void KeyboardMouse::unbindKeyboardButton(const SDL_Scancode &key, const Button &button)
	{
		buttonBindings[key].erase(std::remove(buttonBindings[key].begin(), buttonBindings[key].end(), button), buttonBindings[key].end());
	}

	void KeyboardMouse::bindKeyboardAxis(const SDL_Scancode &key1, const SDL_Scancode &key2, const Axis &axis)
	{
		axisBindings[std::make_pair(key1, key2)].push_back(axis);
	}

	void KeyboardMouse::unbindKeyboardAxis(const SDL_Scancode &key1, const SDL_Scancode &key2, const Axis &axis)
	{
		std::pair<SDL_Scancode, SDL_Scancode> keyPair = std::make_pair(key1, key2);

		axisBindings[keyPair].erase(std::remove(axisBindings[keyPair].begin(), axisBindings[keyPair].end(), axis), axisBindings[keyPair].end());
	}

	void KeyboardMouse::bindMouseButton(const uint8_t &mbutton, const Button &button)
	{
		mouseButtonBindings[mbutton].emplace_back(button);
	}

	void KeyboardMouse::unbindMouseButton(const uint8_t &mbutton, const Button &button)
	{
		mouseButtonBindings[mbutton].erase(std::remove(mouseButtonBindings[mbutton].begin(), mouseButtonBindings[mbutton].end(), button), mouseButtonBindings[mbutton].end());
	}

	void KeyboardMouse::bindMouseAxes(const Axis &axis1, const Axis &axis2)
	{
		mouseAxisBindings.emplace_back(axis1, axis2);
	}

	void KeyboardMouse::unbindMouseAxes(const Axis &axis1, const Axis &axis2)
	{
		mouseAxisBindings.erase(std::remove(mouseAxisBindings.begin(), mouseAxisBindings.end(), std::make_pair(axis1, axis2)), mouseAxisBindings.end());
	}
}