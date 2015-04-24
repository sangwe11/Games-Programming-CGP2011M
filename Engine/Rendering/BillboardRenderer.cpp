#include "BillboardRenderer.h"

namespace Engine
{
	BillboardRenderer::BillboardRenderer(const std::string &texture, const glm::vec2 &scale)
	{
		this->texture = texture;
		this->scale = scale;
		
		// Reserve space
		positions.reserve(100);
	}

	void BillboardRenderer::initialise()
	{
		glGenBuffers(1, &buffer);
		glGenVertexArrays(1, &vaObject);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2000, NULL, GL_DYNAMIC_DRAW);

		glBindVertexArray(vaObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void BillboardRenderer::uninitialise()
	{
		glDeleteBuffers(1, &buffer);
		glDeleteVertexArrays(1, &vaObject);
	}

	// Add a world position point to render another billboard at
	void BillboardRenderer::addBillboard(const glm::vec3 position)
	{
		positions.push_back(position);
	}
}