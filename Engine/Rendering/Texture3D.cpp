#include "Texture3D.h"

namespace Engine
{
	bool Texture3D::load(const std::string &positiveX, const std::string &negativeX, const std::string &positiveY, const std::string &negativeY, const std::string &positiveZ, const std::string &negativeZ, const bool &anisotropicFiltering, const unsigned int &anisotropicSamples)
	{
		// Generate texture
		glGenTextures(1, &texture);

		// Bind GL texture handle
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		// Store texture names
		std::vector<std::string> textures;

		textures.emplace_back(positiveX);
		textures.emplace_back(negativeX);
		textures.emplace_back(positiveY);
		textures.emplace_back(negativeY);
		textures.emplace_back(positiveZ);
		textures.emplace_back(negativeZ);

		// Load textures
		for (unsigned int i = 0; i < 6; ++i)
		{
			SDL_Surface *surface = nullptr;

			// Load texture using SDL_image
			surface = IMG_Load((name + "/" + textures[i]).c_str());

			// Check texture loaded
			if (surface == nullptr)
				return false;

			// Switch on format
			switch (surface->format->format)
			{
			case SDL_PIXELFORMAT_RGB24:
			case SDL_PIXELFORMAT_RGB332:
			case SDL_PIXELFORMAT_RGB444:
			case SDL_PIXELFORMAT_RGB555:
			case SDL_PIXELFORMAT_RGB565:
			case SDL_PIXELFORMAT_RGB888:
				// RGB format
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				break;
			case SDL_PIXELFORMAT_RGBA4444:
			case SDL_PIXELFORMAT_RGBA5551:
			case SDL_PIXELFORMAT_RGBA8888:
				// RGBA format
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
				break;
			case SDL_PIXELFORMAT_BGR24:
			case SDL_PIXELFORMAT_BGR555:
			case SDL_PIXELFORMAT_BGR565:
			case SDL_PIXELFORMAT_BGR888:
				// BGR format
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
				break;
			case SDL_PIXELFORMAT_ABGR1555:
			case SDL_PIXELFORMAT_ABGR4444:
			case SDL_PIXELFORMAT_ABGR8888:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
				break;
			case SDL_PIXELFORMAT_ARGB1555:
			case SDL_PIXELFORMAT_ARGB2101010:
			case SDL_PIXELFORMAT_ARGB4444:
			case SDL_PIXELFORMAT_ARGB8888:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
				break;
			default:
				std::cout << "Unknown texture format: " << SDL_GetPixelFormatName(surface->format->format) << std::endl;
				glDeleteTextures(1, &texture);
				return false;
				break;
			}

			// Free SDL surface
			SDL_FreeSurface(surface);
			surface = nullptr;

		}

		// Set texture params
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Anisotropic filtering if desired
		if (anisotropicFiltering)
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)anisotropicSamples); // Anisotropic filtering

		// Unbind texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		// Texture loaded
		return true;
	}

	void Texture3D::cleanup()
	{
		// Delete texture
		glDeleteTextures(1, &texture);
	}

	const GLuint &Texture3D::get() const
	{
		return texture;
	}

	void Texture3D::use(const unsigned int &unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}
}