#ifndef _BILLBOARDRENDERER_H
#define _BILLBOARDRENDERER_H

#include <string>
#include <GLEW/glew.h>

#include "../EntitySystem/Entity.h"
#include "../Core/Maths.h"

namespace Engine
{
	class BillboardRenderer : public EntitySystem::Component<BillboardRenderer>
	{
		friend class Rendering;

	public:
		BillboardRenderer(const std::string &texture, const glm::vec2 &scale);

		virtual void initialise();
		virtual void uninitialise();
		
		// Add a world position point to render another billboard at
		void addBillboard(const glm::vec3 position);

	private:
		// Settings
		std::string texture;
		glm::vec2 scale;

		// State
		GLuint buffer;
		GLuint vaObject;
		std::vector<glm::vec3> positions;
	};
}
#endif