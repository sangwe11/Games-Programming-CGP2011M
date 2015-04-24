#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SDL/SDL.h>
#include <GLM/glm.hpp>

#include "../EntitySystem/System.h"

namespace Engine
{
	class Display : public EntitySystem::System<Display>
	{
	public:
		Display(const std::string &title, const unsigned int &width, const unsigned int &height, const bool &fullscreen, const bool &vsync);
		virtual ~Display();

		virtual void initialise();

		// Clear the display buffer
		void clear();
		
		// Swap display buffers
		void swap();

		// Get display id
		const int &getId() const;

		// Get display size
		const glm::uvec2 &getSize() const;
		const unsigned int &getWidth() const;
		const unsigned int &getHeight() const;
		const float getAspectRatio() const;

		// Get display title
		const std::string &getTitle() const;

		// Set display size
		void resize(const glm::uvec2 &size);
		void resize(const unsigned int &width, const unsigned int &height);

		// Set display title
		void setTitle(const std::string &title);

		// Set fullscreen
		void setFullscreen(const bool &fullscreen);

	private:
		int id;
		std::string title;
		glm::uvec2 size;
		bool fullscreen;
		glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		SDL_Window *window;
		SDL_GLContext context;
	};
}

#endif