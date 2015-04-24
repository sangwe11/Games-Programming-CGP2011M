#include "FlickeringLight.h"

#include <Core/Maths.h>
#include <Core/Time.h>
#include <EntitySystem/World.h>

namespace Engine
{
	FlickeringLight::FlickeringLight(const float &minIntensity, const float &maxIntensity, const float &minRange, const float &maxRange, const float &delay)
	{
		this->minIntensity = minIntensity;
		this->maxIntensity = maxIntensity;
		this->minRange = minRange;
		this->maxRange = maxRange;
		this->delay = delay;
	}

	void FlickeringLight::onAwake()
	{
		// Get light
		light = entity.getComponent<Engine::PointLight>();
	}

	void FlickeringLight::update()
	{
		if (timer <= 0.0f)
		{
			// Flicker
			float random1 = (rand() % 100) / 100.0f;
			float random2 = (rand() % 100) / 100.0f;

			light->intensity = glm::mix(minIntensity, maxIntensity, random1);
			light->radius = glm::mix(minRange, maxRange, random2);

			// Reset timer
			timer = delay;
		}
		else
		{
			timer -= manager->getWorld().systems.getSystem<Time>()->getDeltaTime();
		}
	}
}