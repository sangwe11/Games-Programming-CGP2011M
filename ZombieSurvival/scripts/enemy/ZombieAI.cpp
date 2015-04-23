#include "ZombieAI.h"
#include <EntitySystem/World.h>
#include <Core/Time.h>

ZombieAI::ZombieAI(const float &walkSpeed, const float &runSpeed) : walkSpeed(walkSpeed), runSpeed(runSpeed)
{
}

void ZombieAI::onAwake()
{
	// Get components
	transform = entity.getComponent<Engine::Transform>();

	// Add sounds
	moanSound = entity.addComponent<Engine::AudioSource>("sounds/zombie_groan_005.mp3", 100);

	// Get player transform
	playerController = entity.getManager().getAllComponents<Engine::PlayerController>(false)[0];
	player = playerController.getEntity().getComponent<Engine::Transform>();
}

void ZombieAI::update()
{
	float deltaTime = manager->getWorld().systems.getSystem<Engine::Time>()->getDeltaTime();

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

	// Move towards player
	transform->position += directionToPlayer * speed * deltaTime;

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

	// Timers
	moanTimer -= deltaTime;
	hitTimer -= deltaTime;

	if (moanTimer <= 0.0f)
	{
		// Set a random time to moan next
		moanTimer = ((rand() % 2000) / 100.0f) + 10.0f;
		moanTimer = glm::clamp(moanTimer, 10.0f, 20.0f);

		// Play a random moan sound effect
		int clip = (rand() % 9) / 3;

		moanSound->play();
	}

	// Check attack player
	if (distanceToPlayer <= 1.0f && hitTimer <= 0.0f)
	{
		playerController->getHealth()->addDamage(20.0f);
		hitTimer = 1.0f;
	}
}