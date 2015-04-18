#include "Mesh.h"
#include "Shader.h"

namespace Engine
{
	bool Mesh::load(aiMesh *mesh)
	{
		std::vector<unsigned int> indices;
		drawCount = 0;

		// Push faces into a indicies list
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
			drawCount += 3;
		}

		// Buffer index data
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Buffer vertex data
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mVertices, GL_STATIC_DRAW);

		// Buffer normal data if present
		if (mesh->mNormals != NULL)
		{
			hasNormals = true;
			glGenBuffers(1, &normalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mNormals, GL_STATIC_DRAW);
		}

		// Buffer tangent data if present
		if (mesh->mTangents != NULL)
		{
			hasTangents = true;
			glGenBuffers(1, &tangentBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mTangents, GL_STATIC_DRAW);
		}

		// Buffer bitangent data if present
		if (mesh->mBitangents != NULL)
		{
			hasBitangents = true;
			glGenBuffers(1, &bitangentBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mBitangents, GL_STATIC_DRAW);
		}

		// Buffer uv data if present
		if (mesh->mTextureCoords != NULL)
		{
			hasUvs = true;
			glGenBuffers(1, &uvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mTextureCoords[0], GL_STATIC_DRAW);
		}

		// Store material index
		materialIndex = mesh->mMaterialIndex;

		// Generate vertex array and bind
		glGenVertexArrays(1, &vaObject);
		glBindVertexArray(vaObject);

		// Setup using shader
		Shader &shader = files->loadFile<Shader>("shaders/test", "shaders/test.vertex", "shaders/test.fragment");

		if (shader.getAttrib("vertex", vertexAttrib))
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glEnableVertexAttribArray(vertexAttrib);
			glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (shader.getAttrib("normal", normalAttrib) && hasNormals)
		{
			glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
			glEnableVertexAttribArray(normalAttrib);
			glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (shader.getAttrib("tangent", tangentAttrib) && hasTangents)
		{
			glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
			glEnableVertexAttribArray(tangentAttrib);
			glVertexAttribPointer(tangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (shader.getAttrib("bitangent", bitangentAttrib) && hasBitangents)
		{
			glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
			glEnableVertexAttribArray(bitangentAttrib);
			glVertexAttribPointer(bitangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (shader.getAttrib("uv", uvAttrib) && hasUvs)
		{
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glEnableVertexAttribArray(uvAttrib);
			glVertexAttribPointer(uvAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}

	void Mesh::cleanup()
	{
		// Delete index and vertex buffers
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &vertexBuffer);

		// Delete other buffers
		if (hasNormals) glDeleteBuffers(1, &normalBuffer);
		if (hasTangents) glDeleteBuffers(1, &tangentBuffer);
		if (hasBitangents) glDeleteBuffers(1, &bitangentBuffer);
		if (hasUvs) glDeleteBuffers(1, &uvBuffer);

		// Delete vertex array
		glDeleteVertexArrays(1, &vaObject);
	}
}