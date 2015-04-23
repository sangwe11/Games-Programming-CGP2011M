#include "HealthPack.h"

void HealthPack::onAwake()
{
	transform = entity.getComponent<Engine::Transform>();
	player = entity.getManager().getTaggedEntity("player").getComponent<Engine::Transform>();
	playerHealth = player.getEntity().getComponent<PlayerHealth>();
	sound = entity.addComponent<Engine::AudioSource>("sounds/pickupsound.mp3", 10, false, false);
}

void HealthPack::update()
{
	glm::vec3 vectorBetween = player->position - transform->position;
	float distance = glm::length(vectorBetween);

	if (distance <= 1.0f)
	{
		// Pick up the health pack
		playerHealth->addHealth(50.0f);

		// Play sound effect
		sound->play();

		// Destroy pickup entity
		entity.destroy();
	}
}