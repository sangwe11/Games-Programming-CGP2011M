#include "Shader.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Rendering.h"

namespace Engine
{
	bool Shader::load()
	{
		std::string linkError, validateError;

		// Create shader program
		program = glCreateProgram();

		// Vertex shader is required
		if (createShader(name, GL_VERTEX_SHADER, vertex))
		{
			// Attach to program
			glAttachShader(program, vertex);
		}
		else
		{
			// Error loading / empty source
			std::cout << "Vertex shader is required" << std::endl;
			return false;
		}

		// Optional fragment shader
		if (createShader(name, GL_FRAGMENT_SHADER, fragment))
		{
			// Attach shader to program
			glAttachShader(program, fragment);
		}

		// Optional geometry shader
		if (createShader(name, GL_GEOMETRY_SHADER, geometry))
		{
			// Attach shader to program
			glAttachShader(program, geometry);
		}

		// Set transform feedback varyings if given
		//if (varyings != nullptr)
		//{
		//	glTransformFeedbackVaryings(program, varyingCount, varyings, GL_INTERLEAVED_ATTRIBS);
		//}

		// Link program
		glLinkProgram(program);

		// Check link status
		if (checkProgramError(program, GL_LINK_STATUS, linkError))
		{
			std::cout << linkError << std::endl;
			return false;
		}

		// Validate program
		if (checkProgramError(program, GL_VALIDATE_STATUS, validateError))
		{
			std::cout << validateError << std::endl;
			return false;
		}

		// Use the shader
		glUseProgram(program);

		// Store all attribs and uniforms for easier lookup
		GLint numActiveAttribs = 0;
		GLint numActiveUniforms = 0;
		GLint numUniformBuffers = 0;

		glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);

		// Get and store active attribs
		for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
		{
			// Grab the resource name and store
			std::vector <GLchar> nameData(256);
			glGetProgramResourceName(program, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);

			// Convert to std::string and strip null chars using c++11 lambda
			std::string name((char*)&nameData[0], nameData.size() - 1);
			name.erase(std::remove_if(name.begin(), name.end(), [](char c){ return (c == '\0'); }), name.end());

			// Get location and store
			GLuint location = glGetAttribLocation(program, &nameData[0]);
			attribs[name] = location;
		}

		glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

		// Get and store active uniforms
		for (int uniform = 0; uniform < numActiveUniforms; ++uniform)
		{
			// Grab the resource name and store
			std::vector <GLchar> nameData(256);
			glGetProgramResourceName(program, GL_UNIFORM, uniform, nameData.size(), NULL, &nameData[0]);

			// Convert to std::string and strip null chars using c++11 lambda
			std::string name((char*)&nameData[0], nameData.size() - 1);
			name.erase(std::remove_if(name.begin(), name.end(), [](char c){ return (c == '\0'); }), name.end());

			// Get location
			GLuint location = glGetUniformLocation(program, &nameData[0]);

			// Check the uniform is not part of a uniform block and store
			if (location != 4294967295)
				uniforms[name] = location;
		}

		// Get and store active uniform buffers
		glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numUniformBuffers);

		for (int buffer = 0; buffer < numUniformBuffers; buffer++)
		{
			std::vector<GLchar> nameData(256);

			// Get the resource name and store in nameData
			glGetProgramResourceName(program, GL_UNIFORM_BLOCK, buffer, nameData.size(), NULL, &nameData[0]);

			// Convert to std::string and strip null chars using c++11 lambda
			std::string name((char*)&nameData[0], nameData.size() - 1);
			name.erase(std::remove_if(name.begin(), name.end(), [](char c){ return (c == '\0'); }), name.end());

			// Get location in shader
			GLint blockIndex = glGetUniformBlockIndex(program, &nameData[0]);

			// Buffer binding
			GLuint bufferBinding;

			// Check if the uniform buffer exists
			if (files->getManager().getSystem<Rendering>()->uniformBufferExists(name))
			{
				// Get the uniform buffer
				UniformBuffer &uniformBuffer = files->getManager().getSystem<Rendering>()->getUniformBuffer(name);

				// Get buffer binding
				bufferBinding = uniformBuffer.getBufferBinding();
			}
			else
			{
				GLint bufferSize;

				// Get block size
				glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);

				// Create the uniform buffer
				UniformBuffer &uniformBuffer = files->getManager().getSystem<Rendering>()->newUniformBuffer(name, bufferSize);

				// Get buffer binding
				bufferBinding = uniformBuffer.getBufferBinding();
			}

			// Set block binding
			if (blockIndex != 4294967295)
				glUniformBlockBinding(program, blockIndex, bufferBinding);
		}

		return true;
	}

	void Shader::cleanup()
	{
		// Detach and delete vertex shader
		glDetachShader(program, vertex);
		glDeleteShader(vertex);

		// Detach and delete fragment shader
		if (fragment)
		{
			glDetachShader(program, fragment);
			glDeleteShader(fragment);
		}

		// Detach and delete geometry shader
		if (geometry)
		{
			glDetachShader(program, geometry);
			glDeleteShader(geometry);
		}

		// Delete shader program
		glDeleteProgram(program);
	}

	const GLuint Shader::id() const
	{
		return program;
	}

	void Shader::use()
	{
		GLint current;
		glGetIntegerv(GL_CURRENT_PROGRAM, &current);

		if (current != program)
		{
			glUseProgram(program);
		}
	}

	bool Shader::getAttrib(const std::string& name, GLuint& attribOut)
	{
		for (auto attrib : attribs)
		{
			if (attrib.first == name)
			{
				attribOut = attrib.second;
				return true;
			}
		}

		// Issue a warning if the attrib wasn't found
		std::cout << "Couldn't find attrib '" << name << "' in shader '" << this->name << "'" << std::endl;

		return false;
	}

	bool Shader::getUniform(const std::string& name, GLuint& uniformOut)
	{
		if (uniforms.find(name) != uniforms.end())
		{
			uniformOut = uniforms[name];
			return true;
		}

		// Issue a warning if the uniform wasn't found
		//std::cout << "Couldn't find uniform '" << name << "' in shader '" << this->name << "'" << std::endl;

		return false;
	}

	void Shader::setUniform(const std::string &name, const glm::mat4 &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniformMatrix4fv(uniform, 1, false, &value[0][0]);
	}

	void Shader::setUniform(const std::string &name, const glm::mat3 &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniformMatrix3fv(uniform, 1, false, &value[0][0]);
	}

	void Shader::setUniform(const std::string &name, const glm::vec4 &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform4fv(uniform, 1, &value[0]);
	}

	void Shader::setUniform(const std::string &name, const glm::vec3 &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform3fv(uniform, 1, &value[0]);
	}

	void Shader::setUniform(const std::string &name, const glm::vec2 &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform2fv(uniform, 1, &value[0]);
	}

	void Shader::setUniform(const std::string &name, const float &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform1fv(uniform, 1, &value);
	}

	void Shader::setUniform(const std::string &name, const int &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform1i(uniform, value);
	}

	void Shader::setUniform(const std::string &name, const unsigned int &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform1i(uniform, value);
	}

	void Shader::setUniform(const std::string &name, const bool &value)
	{
		GLuint uniform;
		if (getUniform(name, uniform)) glUniform1i(uniform, value);
	}

	std::string Shader::sourceFromFile(const std::string &fileName)
	{
		std::ifstream file;
		std::string source;
		std::string line;

		// Open file
		file.open(fileName);

		// Read contents
		if (file.is_open())
		{
			while (file.good())
			{
				// Append line to source
				getline(file, line);
				source.append(line + "\n");
			}
		}

		return source;
	}

	bool Shader::checkError(GLuint shader, GLuint flag, std::string &errorOut)
	{
		GLint status;
		GLchar error[1024] = { 0 };

		// Check status
		glGetShaderiv(shader, flag, &status);

		// Check for error
		if (status == GL_FALSE)
		{
			// Get error
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
			errorOut = error;
			return true;
		}

		// No error
		return false;
	}

	bool Shader::checkProgramError(GLuint program, GLuint flag, std::string &errorOut)
	{
		GLint status;
		GLchar error[1024] = { 0 };

		// Check status
		glGetProgramiv(program, flag, &status);

		if (status == GL_FALSE)
		{
			// Get error
			glGetProgramInfoLog(program, sizeof(error), NULL, error);
			errorOut = error;
			return true;
		}

		// No error
		return false;
	}

	bool Shader::createShader(const std::string &name, GLenum type, GLuint &shader)
	{
		std::string ext;
		std::string compileError;

		// File extension
		switch (type)
		{
		case GL_VERTEX_SHADER:
			ext = ".vertex";
			break;
		case GL_FRAGMENT_SHADER:
			ext = ".fragment";
			break;
		case GL_GEOMETRY_SHADER:
			ext = ".geometry";
			break;
		default:
			ext = "";
			break;
		}

		// Load shader source
		std::string source = sourceFromFile(name + ext);

		// Check source is not empty
		if (source == "")
			return false;

		// Convert to GL types
		const GLchar* GLsource = source.c_str();
		GLint GLsourceLength = source.length();

		// Create shader
		shader = glCreateShader(type);

		// Pass source and compile
		glShaderSource(shader, 1, &GLsource, &GLsourceLength);
		glCompileShader(shader);

		// Check for compile errors
		if (checkError(shader, GL_COMPILE_STATUS, compileError))
		{
			std::cout << compileError << std::endl;
			return false;
		}

		return true;
	}
}