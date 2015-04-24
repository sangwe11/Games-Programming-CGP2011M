#include "EnemyHealth.h"

#include <Rendering/MeshRenderer.h>
#include "EnemyMovement.h"

EnemyHealth::EnemyHealth(const float &startHealth, const float &maxHealth) : currentHealth(startHealth), maxHealth(maxHealth)
{

}

void EnemyHealth::onAwake()
{
	// Get time system
	time = entity.getManager().getWorld().systems.getSystem<Engine::Time>();

	// Get game control
	gameControl = entity.getManager().getTaggedEntity("gamecontrol").getComponent<GameControl>();
}

void EnemyHealth::update()
{
	// Check still alive
	if (currentHealth <= 0.0f && isAlive)
	{
		isAlive = false;
		deathTimer = 2.0f;

		// Play sound

		// Disable components
		//entity.getComponentInChildren<Engine::MeshRenderer>()->disable();
		entity.getComponent<EnemyMovement>()->disable();
	}

	// Delay entity destroy
	if (!isAlive && deathTimer > 0.0f)
	{
		deathTimer -= time->getDeltaTime();
	}
}

void EnemyHealth::addDamage(const float &amount)
{
	currentHealth -= amount;
}