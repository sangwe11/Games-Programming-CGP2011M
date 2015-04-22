#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <GLEW/glew.h>
#include <vector>

#include "../Core/Maths.h"

namespace Engine
{
	class Framebuffer
	{
	public:
		Framebuffer(const unsigned int &width, const unsigned int &height, const unsigned int &noTextures, const bool &depthT);
		Framebuffer(const glm::uvec2 &size, const unsigned int &noTextures, const bool &depthT);
		~Framebuffer();

		void bind(const bool &read = true, const bool &draw = true);
		void unbind(const bool &read = true, const bool &draw = true);

		void bindTextures(const std::vector<unsigned int> &textures);
		void bindDrawbuffers(const std::vector<unsigned int> &textures);
		void bindReadbuffer(const unsigned int &textures);

		void clear(const std::vector<unsigned int> &textures, const bool &depth, const bool &stencil);

	private:
		GLuint fbo;
		std::vector<GLuint> textures;
		GLuint depth;
	};
}

#endif