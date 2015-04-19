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

	SpotLight::SpotLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff, glm::vec3 direction, float angle, float exponent)
	{
		this->color = color;
		this->intensity = intensity;
		this->ambient = ambient;
		this->radius = radius;
		this->falloff = falloff;
		this->direction = direction;
		this->angle = angle;
		this->exponent = exponent;
	}
}