#ifndef _TEXTURE3D_H
#define _TEXTURE3D_H

#include "../Core/Files.h"

#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace Engine
{
	class Texture3D : public File<Texture3D>
	{
	public:
		// Load a 2D texture from file into OpenGL
		virtual bool load(const std::string &positiveX, const std::string &negativeX, const std::string &positiveY, const std::string &negativeY, const std::string &positiveZ, const std::string &negativeZ, const bool &anisotropicFiltering = true, const unsigned int &anisotropicSamples = 16);

		// Cleanup the loaded texture
		virtual void cleanup();

		// Get the OpenGL texture handle
		const GLuint &get() const;

		// Bind the texture to an active texture unit
		void use(const unsigned int &unit);

	private:
		GLuint texture;
	};
}

#endif