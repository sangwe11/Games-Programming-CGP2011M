#ifndef _LIGHT_H
#define _LIGHT_H

#include "../EntitySystem/Entity.h"
#include "../Core/Maths.h"

namespace Engine
{
	class DirectionalLight : public EntitySystem::Component<DirectionalLight>
	{
	public:
		DirectionalLight(glm::vec3 color, float intensity, float ambient, glm::vec3 direction);

		glm::vec3 color;
		float intensity;
		float ambient;
		glm::vec3 direction;
	};

	class PointLight : public EntitySystem::Component<PointLight>
	{
	public:
		PointLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff);

		glm::vec3 color;
		float intensity;
		float ambient;
		float radius;
		float falloff;
	};

	class SpotLight : public EntitySystem::Component<SpotLight>
	{
	public:
		SpotLight(glm::vec3 color, float intensity, float ambient, float range, glm::vec3 direction, float innerAngle, float outerAngle);

		glm::vec3 color;
		float intensity;
		float ambient;
		float range;
		glm::vec3 direction;
		float innerAngle;
		float outerAngle;
	};
}

#endif