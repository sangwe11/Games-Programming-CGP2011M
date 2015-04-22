#ifndef _MESH_H
#define _MESH_H

#include <ASSIMP/mesh.h>
#include <GLEW/glew.h>

#include "../Core/Files.h"

namespace Engine
{
	class Mesh : public File<Mesh>
	{
		friend class Model;

	public:
		virtual bool load(aiMesh *mesh);
		virtual void cleanup();

	private:
		unsigned int drawCount;
		unsigned int materialIndex;

		GLuint indexBuffer;
		GLuint vertexBuffer;
		GLuint normalBuffer;
		GLuint tangentBuffer;
		GLuint bitangentBuffer;
		GLuint colorBuffer;
		GLuint uvBuffer;
		GLuint vaObject;

		GLuint vertexAttrib;
		GLuint normalAttrib;
		GLuint tangentAttrib;
		GLuint bitangentAttrib;
		GLuint colorAttrib;
		GLuint uvAttrib;

		bool hasNormals = false;
		bool hasTangents = false;
		bool hasBitangents = false;
		bool hasUvs = false;
	};
}

#endif