#ifndef _RENDERINGPRIMATIVE_H
#define _RENDERINGPRIMATIVE_H

#include <vector>
#include <GLEW/glew.h>

#include "../Core/Maths.h"
#include "Shader.h"

namespace Engine
{
	class RenderingPrimative
	{
	public:
		~RenderingPrimative();
		GLuint vaObject;
		unsigned int drawCount = 0;

	protected:
		void Setup(const unsigned int &vertexCount, glm::vec3 *vertices, glm::vec2 *uvs, const unsigned int &drawCount, unsigned int *indicies, Shader &shader);

		GLuint indexBuffer;
		GLuint vertexBuffer;
		GLuint uvBuffer;
	};

	class RenderingQuad : public RenderingPrimative
	{
	public:
		RenderingQuad(Shader &shader);
	};

	class RenderingCube : public RenderingPrimative
	{
	public:
		RenderingCube(Shader &shader);
	};

	class RenderingSphere : public RenderingPrimative
	{
	public:
		RenderingSphere(Shader &shader, unsigned int level);

	private:
		struct Face
		{
		public:
			typedef std::vector<Face> List;

			Face(unsigned int v1, unsigned int v2, unsigned int v3);

			unsigned int v1;
			unsigned int v2;
			unsigned int v3;
		};

		int addVertex(glm::vec3 point);
		void addFace(Face face);
		int getMiddlePoint(glm::vec3 point1, glm::vec3 point2);
		void create(unsigned int recursionLevel);

		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indicies;
		unsigned int index;
	};
}

#endif