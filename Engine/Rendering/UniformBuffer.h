#ifndef _UNIFORMBUFFER_H
#define _UNIFORMBUFFER_H

#include <string>
#include <GLEW/glew.h>

namespace Engine
{
	class UniformBuffer
	{
	public:
		UniformBuffer(const std::string &name, const unsigned int &size, const GLuint &bufferBinding);
		~UniformBuffer();

		const std::string &getName() { return name; }
		const unsigned int &getSize() { return size; }
		const GLuint &getBuffer() { return buffer; }
		const GLuint &getBufferBinding() { return bufferBinding; }

	private:
		std::string name;
		unsigned int size;
		GLuint buffer;
		GLuint bufferBinding;
	};
}

#endif