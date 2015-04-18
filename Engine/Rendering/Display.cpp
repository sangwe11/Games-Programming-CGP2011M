#include "Display.h"

#include <GLEW/glew.h>

namespace Engine
{
	Display::Display(const std::string &title, const unsigned int &width, const unsigned int &height, const bool &fullscreen, const bool &vsync)
	{
		// Store window properties
		this->title = title;
		this->size = glm::uvec2(width, height);

		// Create window
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

		// Set fullscreen if desired
		setFullscreen(fullscreen);

		// Store window ID
		id = SDL_GetWindowID(window);

		// Create GL context
		context = SDL_GL_CreateContext(window);

		// Using modern OpenGL
		glewExperimental = GL_TRUE;

		// Initialise glew
		GLenum glew = glewInit();

		// Check glew status
		if (glew != GLEW_OK)
		{
			// Error
			std::cout << "Glew initialisation failed" << std::endl;
			std::cout << glewGetErrorString(glew) << std::endl;

			// Exit
			exit(1);
		}
		else
		{
			std::cout << "Glew initialised." << std::endl;

			// Enable back face culling with counter-clockwise winding for front faces
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			// Enable depth testing
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			// Enable vsync
			if (vsync) SDL_GL_SetSwapInterval(1);
			else SDL_GL_SetSwapInterval(0);

			// Print debug information
			std::cout << glGetString(GL_VERSION) << std::endl;
			std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
			std::cout << glGetString(GL_VENDOR) << std::endl;
			std::cout << glGetString(GL_RENDERER) << std::endl;
		}
	}

	Display::~Display()
	{
		// Delete context
		SDL_GL_DeleteContext(context);

		// Delete window
		SDL_DestroyWindow(window);
	}

	void Display::initialise()
	{
		// Add functions to update loop
		addUpdateFunction(&Display::clear, *this, 0, false);
		addUpdateFunction(&Display::swap, *this, 100, false);
	}

	void Display::clear()
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Display::swap()
	{
		SDL_GL_SwapWindow(window);
	}

	const int &Display::getId() const
	{
		return id;
	}

	const glm::uvec2 &Display::getSize() const
	{
		return size;
	}

	const unsigned int &Display::getWidth() const
	{
		return size.x;
	}

	const unsigned int &Display::getHeight() const
	{
		return size.y;
	}

	const float Display::getAspectRatio() const
	{
		return (float)size.x / (float)size.y;
	}

	const std::string &Display::getTitle() const
	{
		return title;
	}

	void Display::resize(const glm::uvec2 &size)
	{
		// Store new size
		this->size = size;

		// Resize viewport
		glViewport(0, 0, size.x, size.y);
	}

	void Display::resize(const unsigned int &width, const unsigned int &height)
	{
		resize(glm::uvec2(width, height));
	}

	void Display::setTitle(const std::string &title)
	{
		// Store the new title
		this->title = title;

		SDL_SetWindowTitle(window, this->title.c_str());
	}

	void Display::setFullscreen(const bool &fullscreen)
	{
		// Set fullscreen
		if (fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else SDL_SetWindowFullscreen(window, 0);

		// Store flag
		this->fullscreen = fullscreen;
	}
}