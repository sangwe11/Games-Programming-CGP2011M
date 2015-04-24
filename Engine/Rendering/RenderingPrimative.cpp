#include "RenderingPrimative.h"

namespace Engine
{
	RenderingPrimative::~RenderingPrimative()
	{
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &uvBuffer);
		glDeleteVertexArrays(1, &vaObject);
	}

	void RenderingPrimative::Setup(const unsigned int &vertexCount, glm::vec3 *vertices, glm::vec2 *uvs, const unsigned int &drawCount, unsigned int *indicies, Shader &shader)
	{
		GLuint vertexAttrib, uvAttrib;

		// Generate index buffer
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

		// Generate vertex buffer
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

		// Generate uv buffer if needed
		if (uvs != nullptr)
		{
			glGenBuffers(1, &uvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec2), uvs, GL_STATIC_DRAW);
		}

		// Generate vertex array object
		glGenVertexArrays(1, &vaObject);

		// Bind
		glBindVertexArray(vaObject);

		// Enable vertex attrib
		if (shader.getAttrib("vertex", vertexAttrib))
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glEnableVertexAttribArray(vertexAttrib);
			glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Enable uv attrib if needed
		if (shader.getAttrib("uv", uvAttrib) && uvs != nullptr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glEnableVertexAttribArray(uvAttrib);
			glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind element array buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		// Unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	RenderingQuad::RenderingQuad(Shader &shader)
	{
		// Generate a quad to use for rendering
		unsigned int vertexCount = 4;
		glm::vec3 vertices[4] = { glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) };
		glm::vec2 uvs[4] = { glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };

		// Indicies
		drawCount = 6;
		unsigned int indicies[6] = { 0, 1, 2, 3, 1, 0 };

		// Setup
		Setup(vertexCount, vertices, uvs, drawCount, indicies, shader);
	}

	RenderingCube::RenderingCube(Shader &shader)
	{
		unsigned int vertexCount = 36;
		glm::vec3 vertices[36] = { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f) };
		glm::vec2 uvs[36] = { glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)};

		drawCount = 36;
		unsigned int indicies[36] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 36, 34, 35, 36 };

		// Setup
		Setup(vertexCount, vertices, uvs, drawCount, indicies, shader);

	}

	RenderingSphere::RenderingSphere(Shader &shader, unsigned int level)
	{
		// Create a sphere
		create(level);

		unsigned int vertexCount = vertices.size();
		drawCount = indicies.size();
		std::vector<glm::vec2> uvs(vertexCount);

		Setup(vertexCount, &vertices[0], &uvs[0], drawCount, &indicies[0], shader);
	}

	int RenderingSphere::addVertex(glm::vec3 point)
	{
		float length = glm::length(point);
		vertices.push_back(glm::vec3(point.x / length, point.y / length, point.z / length));
		return index++;
	}

	void RenderingSphere::addFace(Face face)
	{
		indicies.push_back(face.v1);
		indicies.push_back(face.v2);
		indicies.push_back(face.v3);
	}

	int RenderingSphere::getMiddlePoint(glm::vec3 point1, glm::vec3 point2)
	{
		// Calculate middle point
		glm::vec3 middle;
		middle.x = (point1.x + point2.x) / 2.0f;
		middle.y = (point1.y + point2.y) / 2.0f;
		middle.z = (point1.z + point2.z) / 2.0f;

		// Make sure vertex is on unit sphere
		return addVertex(middle);
	}

	void RenderingSphere::create(unsigned int recursionLevel)
	{
		index = 0;
		float t = (1.0f + glm::sqrt(5.0f)) / 2.0f;
		Face::List faces;

		// Create 12 vertices of icosahedron
		addVertex(glm::vec3(-1.0f, t, 0.0f));
		addVertex(glm::vec3(1.0f, t, 0.0f));
		addVertex(glm::vec3(-1.0f, -t, 0.0f));
		addVertex(glm::vec3(1.0f, -t, 0.0f));

		addVertex(glm::vec3(0.0f, -1.0f, t));
		addVertex(glm::vec3(0.0f, 1.0f, t));
		addVertex(glm::vec3(0.0f, -1.0f, -t));
		addVertex(glm::vec3(0.0f, 1.0f, -t));

		addVertex(glm::vec3(t, 0.0f, -1.0));
		addVertex(glm::vec3(t, 0.0f, 1.0));
		addVertex(glm::vec3(-t, 0.0f, -1.0));
		addVertex(glm::vec3(-t, 0.0f, 1.0));

		// 5 faces around point 0
		faces.emplace_back(0, 11, 5);
		faces.emplace_back(0, 5, 1);
		faces.emplace_back(0, 1, 7);
		faces.emplace_back(0, 7, 10);
		faces.emplace_back(0, 10, 11);

		// 5 adjacent faces
		faces.emplace_back(1, 5, 9);
		faces.emplace_back(5, 11, 4);
		faces.emplace_back(11, 10, 2);
		faces.emplace_back(10, 7, 6);
		faces.emplace_back(7, 1, 8);

		// 5 faces around point 3
		faces.emplace_back(3, 9, 4);
		faces.emplace_back(3, 4, 2);
		faces.emplace_back(3, 2, 6);
		faces.emplace_back(3, 6, 8);
		faces.emplace_back(3, 8, 9);

		// 5 adjacent face
		faces.emplace_back(4, 9, 5);
		faces.emplace_back(2, 4, 11);
		faces.emplace_back(6, 2, 10);
		faces.emplace_back(8, 6, 7);
		faces.emplace_back(9, 8, 1);

		// Refine
		for (unsigned int i = 0; i < recursionLevel; ++i)
		{
			Face::List newFaces;

			for (Face face : faces)
			{
				// Replace triangle with 4 triangles
				unsigned int a = getMiddlePoint(vertices[face.v1], vertices[face.v2]);
				unsigned int b = getMiddlePoint(vertices[face.v2], vertices[face.v3]);
				unsigned int c = getMiddlePoint(vertices[face.v3], vertices[face.v1]);

				// Add 4 new faces to new face list
				newFaces.emplace_back(face.v1, a, c);
				newFaces.emplace_back(face.v2, b, a);
				newFaces.emplace_back(face.v3, c, b);
				newFaces.emplace_back(a, b, c);
			}

			// Replace face list with new face list
			faces = newFaces;
		}

		// Add triangles to indicies list
		for (Face face : faces)
			addFace(face);
	}

	RenderingSphere::Face::Face(unsigned int v1, unsigned int v2, unsigned int v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
}