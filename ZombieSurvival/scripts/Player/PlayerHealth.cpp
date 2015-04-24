#include "PlayerHealth.h"

PlayerHealth::PlayerHealth(const float &startHealth, const float &maxHealth, const std::string &healthPrefix) : currentHealth(startHealth), maxHealth(maxHealth), healthPrefix(healthPrefix), isAlive(true)
{

}

void PlayerHealth::onAwake()
{
	// Add health UI component
	healthUI = entity.addComponent<Engine::UILabel>(healthPrefix + std::to_string((int)currentHealth), "fonts/Vera.ttf", 30, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(-0.97f, 0.92f));

	// Add sounds
	EntitySystem::Entity sound = entity.addChild();
	//hitSound = sound.addComponent<Engine::AudioSource>("");
}

void PlayerHealth::update()
{
	// Check for death
	if (currentHealth <= 0.0f)
		isAlive = false;

	// Update health UI
	healthUI->setText(healthPrefix + std::to_string((int)currentHealth));

	// Change color based on remaining health
	healthUI->setFontColor(glm::mix(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), currentHealth / maxHealth));
}

void PlayerHealth::onDestroy()
{
	// Remove healthUI component
	healthUI.remove();
}

void PlayerHealth::addDamage(const float &amount)
{
	currentHealth -= amount;
}

void PlayerHealth::addHealth(const float &amount)
{
	currentHealth += amount;
}