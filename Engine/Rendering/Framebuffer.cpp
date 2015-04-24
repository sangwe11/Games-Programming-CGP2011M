#include "Framebuffer.h"
#include <iostream>

namespace Engine
{
	Framebuffer::Framebuffer(const unsigned int &width, const unsigned int &height, const unsigned int &noTextures, const bool &depthT)
	{
		// Generate frame buffer object
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//  Textures
		for (unsigned int i = 0; i < noTextures; ++i)
		{
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);

			// Bind texture
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

			// Set params
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Attach to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);

			// Unbind
			glBindTexture(GL_TEXTURE_2D, 0);

			// Add to texture list
			textures.push_back(texture);
		}

		// Depth texture?
		if (depthT)
		{
			// Generate texture
			glGenTextures(1, &depth);

			// Depth texture
			glBindTexture(GL_TEXTURE_2D, depth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

			// Set params
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Attach to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

			// Unbind
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Check status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Incomplete framebuffer: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

		// Unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::Framebuffer(const glm::uvec2 &size, const unsigned int &noTextures, const bool &depthT) : Framebuffer(size.x, size.y, noTextures, depthT)
	{
	}

	Framebuffer::~Framebuffer()
	{
		// Clean up textures
		for (unsigned int i = 0; i < textures.size(); ++i)
			glDeleteTextures(1, &textures[i]);

		if (depth)
			glDeleteTextures(1, &depth);

		// Delete frame buffer object
		glDeleteFramebuffers(1, &fbo);
	}

	void Framebuffer::bind(const bool &read, const bool &draw)
	{
		if (read && draw)
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		else if (read)
			glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		else if (draw)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

	void Framebuffer::unbind(const bool &read, const bool &draw)
	{
		if (read && draw)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		else if (read)
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		else if (draw)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Framebuffer::bindTextures(const std::vector<unsigned int> &textures, const bool &depthT)
	{
		for (unsigned int i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, this->textures[textures[i]]);
		}

		if (depthT)
		{
			glActiveTexture(GL_TEXTURE0 + textures.size());
			glBindTexture(GL_TEXTURE_2D, depth);
			glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		}
	}

	void Framebuffer::bindDrawbuffers(const std::vector<unsigned int> &textures)
	{
		std::vector<GLenum> drawBuffers;

		if (textures.size() <= 0)
		{
			glDrawBuffer(GL_NONE);
			return;
		}

		for (unsigned int i = 0; i < textures.size(); ++i)
		{
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + textures[i]);
		}

		glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
	}

	void Framebuffer::bindReadbuffer(const unsigned int &textures)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + textures);
	}

	void Framebuffer::clear(const std::vector<unsigned int> &textures, const bool &depth, const bool &stencil)
	{
		// Bind and clear
		bind(false, true);
		bindDrawbuffers(textures);

		if (depth && stencil)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		else if (depth)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else if (stencil)
			glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		else
			glClear(GL_COLOR_BUFFER_BIT);
	}
}