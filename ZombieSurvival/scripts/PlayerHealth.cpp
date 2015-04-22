#include "PlayerHealth.h"

#include <Core/Maths.h>
#include <Core/Time.h>
#include <Core/Input.h>
#include <Core/Controller.h>
#include <Core/KeyboardMouse.h>
#include <EntitySystem/World.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/Light.h>
#include <Audio/AudioListener.h>

PlayerHealth::PlayerHealth(const float &startHealth, const float &maxHealth, const bool &regenerate, const float &regenerateRate)
{
	this->currentHealth = startHealth;
	this->maxHealth = maxHealth;
	this->regenerate = regenerate;
	this->regenerateRate = regenerateRate;
	this->alive = true;
}

void PlayerHealth::onAwake()
{
	// Add UI label
	healthLabel = entity.addComponent<Engine::UILabel>("Health: " + std::to_string(currentHealth) + "%", "fonts/Vera.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(-0.97f, 0.92f));
}

void PlayerHealth::update()
{
	// Regenerate health?
	if (regenerate)
		currentHealth += regenerateRate * manager->getWorld().systems.getSystem<Engine::Time>()->getDeltaTime();

	// Update health label
	healthLabel->setText("Health: " + std::to_string((int)currentHealth) + "%");

	// Set color based on health
	if (currentHealth <= 20.0f)
		healthLabel->setFontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	else
		healthLabel->setFontColor(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

	// Check death
	if (currentHealth <= 0.0f)
		alive = false;

	// Clamp health
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}