#include "PlayerTorch.h"

#include <string>

#include <EntitySystem/World.h>
#include <Core/Time.h>

PlayerTorch::PlayerTorch(const float &drainRate)
{
	this->drainRate = drainRate;
}

void PlayerTorch::onAwake()
{
	// Components
	light = entity.addComponent<Engine::SpotLight>(glm::vec3(1.0f), 1.0f, 0.0f, 40.0f, glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, 20.0f);
	batteryLabel = entity.addComponent<Engine::UILabel>("Battery: " + std::to_string((int)batteryRemaining) + "%", "fonts/Vera.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.65f, 0.92f));

	// State
	batteryRemaining = 100.0f;
	on = false;
}

void PlayerTorch::update()
{
	float deltaTime = manager->getWorld().systems.getSystem<Engine::Time>()->getDeltaTime();

	// Does the torch have charge?
	hasCharge = (batteryRemaining > 0.0f);

	// Turn off if the torch is on and its ran out of charge
	if (!hasCharge && on)
		toggle();

	if (on)
	{
		// Drain the battery when the torch is on
		batteryRemaining -= drainRate * deltaTime;

		// Make sure the light is on
		if (!light->isEnabled())
			light->enable();
	}
	else
	{
		// Make sure the light is off
		if (light->isEnabled())
			light->disable();
	}

	// Update battery label
	batteryLabel->setText("Battery: " + std::to_string((int)batteryRemaining) + "%");
}

void PlayerTorch::toggle()
{
	if (on)
		on = false;
	else if (!on && hasCharge)
		on = true;
}