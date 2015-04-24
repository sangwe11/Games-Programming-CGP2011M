#ifndef _SHADER_H
#define _SHADER_H

#include <string>
#include <map>
#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include "../Core/Files.h"

namespace Engine
{
	class Shader : public File<Shader>
	{
	public:
		virtual bool load(const GLchar *varyings[] = nullptr, unsigned int varyingCount = 0);
		virtual void cleanup();

		const GLuint id() const;
		void use();

		// Get attrib by name
		bool getAttrib(const std::string &name, GLuint &attribOut);

		// Get uniform by name
		bool getUniform(const std::string &name, GLuint &uniformOut);

		// Set uniform by name
		void setUniform(const std::string &name, const glm::mat4 &value);
		void setUniform(const std::string &name, const glm::mat3 &value);
		void setUniform(const std::string &name, const glm::vec4 &value);
		void setUniform(const std::string &name, const glm::vec3 &value);
		void setUniform(const std::string &name, const glm::vec2 &value);
		void setUniform(const std::string &name, const float &value);
		void setUniform(const std::string &name, const int &value);
		void setUniform(const std::string &name, const unsigned int &value);
		void setUniform(const std::string &name, const bool &value);

	private:
		GLuint program;
		GLuint vertex;
		GLuint fragment;
		GLuint geometry;

		std::map<std::string, GLuint> attribs;
		std::map<std::string, GLuint> uniforms;

		// Helper functions
		static std::string sourceFromFile(const std::string &fileName);
		static bool checkError(GLuint shader, GLuint flag, std::string &errorOut);
		static bool checkProgramError(GLuint program, GLuint flag, std::string &errorOut);
		static bool createShader(const std::string &name, GLenum type, GLuint &shader);
	};
}

#endif