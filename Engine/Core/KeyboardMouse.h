#ifndef _KEYBOARDMOUSE_H
#define _KEYBOARDMOUSE_H

#include <vector>
#include <map>

#include "Input.h"

namespace Engine
{
	class KeyboardMouse : public Device<KeyboardMouse>
	{
	public:
		KeyboardMouse();

		virtual void update();
		virtual void handle(const SDL_Event &e);

		void bindKeyboardButton(const SDL_Scancode &key, const Button &button);
		void unbindKeyboardButton(const SDL_Scancode &key, const Button &button);

		void bindKeyboardAxis(const SDL_Scancode &key1, const SDL_Scancode &key2, const Axis &axis);
		void unbindKeyboardAxis(const SDL_Scancode &key1, const SDL_Scancode &key2, const Axis &axis);

		void bindMouseButton(const uint8_t &mbutton, const Button &button);
		void unbindMouseButton(const uint8_t &mbutton, const Button &button);

		void bindMouseAxes(const Axis &axis1, const Axis &axis2);
		void unbindMouseAxes(const Axis &axis1, const Axis &axis2);

	private:
		const Uint8 *keystate;

		// Keyboard button bindings
		std::map<SDL_Scancode, std::vector<Button>> buttonBindings;

		// Keyboard axis bindings
		std::map<std::pair<SDL_Scancode, SDL_Scancode>, std::vector<Axis>> axisBindings;

		// Mouse button bindings
		std::map<uint8_t, std::vector<Button>> mouseButtonBindings;

		// Mouse axis bindings
		std::vector<std::pair<Axis, Axis>> mouseAxisBindings;
	};
}

#endif