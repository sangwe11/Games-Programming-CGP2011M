#include "PlayerAccessories.h"

#include <string>

#include <EntitySystem/World.h>
#include <Core/Time.h>
#include <Core/Transform.h>
#include <Core/Input.h>
#include <Core/KeyboardMouse.h>
#include <Core/Controller.h>

#include "../../Post Processing/NightVision.h"

PlayerAccessories::PlayerAccessories(const float &battery, const float &torchDrain, const float &nightVisionDrain) : batteryRemaining(battery), torchDrain(torchDrain), nightVisionDrain(nightVisionDrain)
{
}

void PlayerAccessories::onAwake()
{
	// Add torch as child for offset from camera
	EntitySystem::Entity torch = entity.addChild();
	torch.addComponent<Engine::Transform>(glm::vec3(0.5f, -0.5f, 1.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	// Components
	light = torch.addComponent<Engine::SpotLight>(glm::vec3(1.0f), 1.0f, 0.0f, 40.0f, glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, 20.0f);
	camera = entity.getComponent<Engine::Camera>();
	batteryUI = entity.addComponent<Engine::UILabel>("Battery: " + std::to_string((int)batteryRemaining), "fonts/Vera.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.65f, 0.92f));

	// Add nightvision post processing effect
	camera->addPostProcessingEffect<NightVision>();

	// Disable torch
	light->disable();

	// State
	torchOn = false;
	nightVisionOn = false;
}

void PlayerAccessories::update()
{
	float deltaTime = manager->getWorld().systems.getSystem<Engine::Time>()->getDeltaTime();
	Engine::Input::Handle input = manager->getWorld().systems.getSystem<Engine::Input>();

	// Handle input
	// Keyboard and controller
	if (input->exists<Engine::Controller>(0))
	{
		Engine::Controller &c = input->get<Engine::Controller>(0);

		if (c.getButtonDown("special1"))
			toggleTorch();

		if (c.getButtonDown("special2"))
			toggleNightVision();
	}
	else
	{
		Engine::KeyboardMouse &k = input->get<Engine::KeyboardMouse>(0);

		// Fallback to keyboard and mouse
		if (k.getButtonDown("special1"))
			toggleTorch();

		if (k.getButtonDown("special2"))
			toggleNightVision();
	}

	// Any remaining charge?
	hasCharge = (batteryRemaining > 0.0f);

	// Torch
	if (torchOn)
	{
		// Drain the battery
		batteryRemaining -= torchDrain * deltaTime;

		// Make sure the torch is on
		if (!light->isEnabled())
			light->enable();

		// Turn torch off if there is no charge left
		if (!hasCharge)
			toggleTorch();
	}
	else
	{
		// Make sure the torch is off
		if (light->isEnabled())
			light->disable();
	}

	// Night vision
	if (nightVisionOn)
	{
		// Drain the battery
		batteryRemaining -= nightVisionDrain * deltaTime;

		// Make sure the night vision is enabled
		if (!camera->getPostProcessingEffect<NightVision>().isEnabled())
			camera->getPostProcessingEffect<NightVision>().enable();

		// Turn night vision off if there is no charge left
		if (!hasCharge)
			toggleNightVision();
	}
	else
	{
		// Make sure the night vision is disabled
		if (camera->getPostProcessingEffect<NightVision>().isEnabled())
			camera->getPostProcessingEffect<NightVision>().disable();
	}

	// Update battery UI
	batteryUI->setText("Battery: " + std::to_string((int)batteryRemaining));
}

void PlayerAccessories::toggleTorch()
{
	if (torchOn)
		torchOn = false;
	else if (!torchOn && hasCharge)
		torchOn = true;
}

void PlayerAccessories::toggleNightVision()
{
	if (nightVisionOn)
		nightVisionOn = false;
	else if (!nightVisionOn && hasCharge)
		nightVisionOn = true;
}