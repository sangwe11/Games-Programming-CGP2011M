#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include "../Core/Files.h"

#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace Engine
{
	class Texture2D : public File<Texture2D>
	{
	public:
		// Load a 2D teture from file into OpenGL
		virtual bool load(const bool &mipmaps, const bool &anisotropicFiltering, const unsigned int &anisotropicSamples);

		// Cleanup the loaded texture
		virtual void cleanup();

		// Get the OpenGL texture handle
		const GLuint &get() const;

	private:
		GLuint texture;
	};
}

#endif