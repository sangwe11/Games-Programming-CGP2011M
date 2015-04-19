#include "Light.h"

namespace Engine
{
	DirectionalLight::DirectionalLight(glm::vec3 color, float intensity, float ambient, glm::vec3 direction)
	{
		this->color = color;
		this->intensity = intensity;
		this->ambient = ambient;
		this->direction = direction;
	}

	PointLight::PointLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff)
	{
		this->color = color;
		this->intensity = intensity;
		this->ambient = ambient;
		this->radius = radius;
		this->falloff = falloff;
	}

	SpotLight::SpotLight(glm::vec3 color, float intensity, float ambient, float range, glm::vec3 direction, float innerAngle, float outerAngle)
	{
		this->color = color;
		this->intensity = intensity;
		this->ambient = ambient;
		this->range = range;
		this->direction = direction;
		this->innerAngle = innerAngle;
		this->outerAngle = outerAngle;
	}
}