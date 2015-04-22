#include "UniformBuffer.h"

namespace Engine
{
	UniformBuffer::UniformBuffer(const std::string &name, const unsigned int &size, const GLuint &bufferBinding) : name(name), size(size), bufferBinding(bufferBinding)
	{
		// Generate buffer
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

		// Bind
		glBindBufferRange(GL_UNIFORM_BUFFER, bufferBinding, buffer, 0, size);
	}

	UniformBuffer::~UniformBuffer()
	{
		// Delete buffer
		glDeleteBuffers(1, &buffer);
	}
}