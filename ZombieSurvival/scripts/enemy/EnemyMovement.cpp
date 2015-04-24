#include "EnemyMovement.h"

EnemyMovement::EnemyMovement(const float &walkSpeed, const float &runSpeed) : walkSpeed(walkSpeed), runSpeed(runSpeed)
{

}

void EnemyMovement::onAwake()
{
	// Get required systems
	time = entity.getManager().getWorld().systems.getSystem<Engine::Time>();

	// Get components
	transform = entity.getComponent<Engine::Transform>();
	player = entity.getManager().getTaggedEntity("player").getComponent<Engine::Transform>();
	playerHealth = player.getEntity().getComponent<PlayerHealth>();

	// Add sounds
	EntitySystem::Entity sound1 = entity.addChild();
	sound1.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	moanSounds[0] = sound1.addComponent<Engine::AudioSource>("sounds/zombie_groan_004.mp3", 100);

	EntitySystem::Entity sound2 = entity.addChild();
	sound2.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	moanSounds[1] = sound2.addComponent<Engine::AudioSource>("sounds/zombie_groan_005.mp3", 100);

	EntitySystem::Entity sound3 = entity.addChild();
	sound3.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	moanSounds[2] = sound3.addComponent<Engine::AudioSource>("sounds/zombie_groan_006.mp3", 100);

	// Set timers
	hitTimer = 5.0f;
	moanTimer = 5.0f;
}

void EnemyMovement::update()
{
	// Decrement timers
	hitTimer -= time->getDeltaTime();
	moanTimer -= time->getDeltaTime();

	// get direction and distance to player
	glm::vec3 directionToPlayer = player->positionWorld() - transform->positionWorld();
	float distanceToPlayer = glm::length(directionToPlayer);
	directionToPlayer = glm::normalize(directionToPlayer);

	// Walking or running?
	float speed;

	if (distanceToPlayer <= 10.0f)
		speed = runSpeed;
	else
		speed = walkSpeed;

	// Look at player
	float dot = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), directionToPlayer);

	if (glm::abs(dot - (-1.0f)) < 0.000001f)
	{
		transform->rotation = glm::angleAxis(180.0f, transform->up());
	}
	else if (glm::abs(dot - 1.0f) < 0.000001f)
	{
		transform->rotation = glm::quat();
	}
	else
	{
		glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), directionToPlayer));
		float angle = acos(dot);
		transform->rotation = glm::normalize(glm::angleAxis(angle, axis));
	}

	// Move towards player if we're not close
	if (distanceToPlayer > 1.0f)
	{
		transform->position += directionToPlayer * speed * time->getDeltaTime();
	}
	else
	{
		// Attack the player
		if (hitTimer <= 0.0f)
		{
			playerHealth->addDamage(10.0f);
			hitTimer = ((rand() % 200) / 100.0f) + 2.0f;
		}
	}

	// Zombie moaning
	if (moanTimer <= 0.0f)
	{
		// Set a random time to moan next
		moanTimer = ((rand() % 1200) / 100.0f);
		moanTimer = glm::clamp(moanTimer, 4.0f, 12.0f);

		// Play a random moan sound effect
		int clip = (rand() % 9) / 3;

		moanSounds[clip]->play();
	}
}