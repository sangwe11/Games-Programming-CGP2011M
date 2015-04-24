#include "PlayerMovement.h"

#include <Core/Controller.h>
#include <Core/KeyboardMouse.h>

#include "PlayerHealth.h"
#include "../Controllers/GameControl.h"
#include "../Enemy/EnemySpawning.h"
#include "../Enemy/EnemyHealth.h"

PlayerMovement::PlayerMovement(const float &walkSpeed, const float &runSpeed, const float &jumpHeight) : walkSpeed(walkSpeed), runSpeed(runSpeed), jumpHeight(jumpHeight)
{
}

void PlayerMovement::onAwake()
{
	// Get transform components
	transform = entity.getComponent<Engine::Transform>();
	axe = entity.getManager().getTaggedEntity("axe").getComponent<Engine::Transform>();

	// Get required systems
	input = entity.getManager().getWorld().systems.getSystem<Engine::Input>();
	time = entity.getManager().getWorld().systems.getSystem<Engine::Time>();

	// Add sounds
	EntitySystem::Entity walkS = entity.addChild();
	walkS.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	stepSound = walkS.addComponent<Engine::AudioSource>("sounds/footsteps.mp3", 50, true, false);

	EntitySystem::Entity attackS = entity.addChild();
	attackS.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	attackSound = attackS.addComponent<Engine::AudioSource>("sounds/axe_hit.mp3", 50, false, false);

	// Capture mouse
	mouseCaptured = true;
	SDL_SetRelativeMouseMode((SDL_bool)mouseCaptured);
}

void PlayerMovement::update()
{
	// Input states
	glm::vec2 move;
	bool sprint;
	bool jump;
	bool toggleLight;
	bool attack;
	bool quit;

	// Keyboard and controller
	if (input->exists<Engine::Controller>(0))
	{
		Engine::Controller &c = input->get<Engine::Controller>(0);

		// Use controller if present
		move.x = c.getAxis("moveX");
		move.y = c.getAxis("moveY");
		sprint = c.getButton("sprint");
		jump = c.getButtonDown("jump");
		toggleLight = c.getButtonDown("special1");
		attack = c.getButtonDown("back");
		quit = c.getButtonDown("quit");
	}
	else
	{
		Engine::KeyboardMouse &k = input->get<Engine::KeyboardMouse>(0);

		// Fallback to keyboard and mouse
		move.x = k.getAxis("moveX");
		move.y = k.getAxis("moveY");
		sprint = k.getButton("sprint");
		jump = k.getButtonDown("jump");
		toggleLight = k.getButtonDown("special1");
		attack = k.getButtonDown("fire");
		quit = k.getButtonDown("quit");

		// Toggle mouse capture with M key
		if (k.getButtonDown("mouseCapture"))
		{
			mouseCaptured = !mouseCaptured;
			SDL_SetRelativeMouseMode((SDL_bool)mouseCaptured);
		}

	}

	// Check for quit
	if (quit)
		input->triggerQuit();

	// Some basic player movement
	glm::vec3 forward = transform->forward();
	forward.y = 0.0f;
	forward = glm::normalize(forward);

	// Calculate direction
	glm::vec3 direction;
	direction += forward * -move.y;
	direction += transform->right() * move.x;

	// Is there a direction?
	if (direction.x != 0.0f || direction.z != 0.0f)
	{
		// Do we need to normalise?
		if (glm::length(direction) > 1.0f)
			direction = glm::normalize(direction);

		// Add movement
		if (sprint)
			transform->position += direction * runSpeed * time->getDeltaTime();
		else
			transform->position += direction * walkSpeed * time->getDeltaTime();

		// Play step sound
		stepSound->play();
	}
	else
	{
		// Stop step sound
		stepSound->stop();
	}

	// Click to attack
	if (attack && attackTimer <= 0.0f)
	{
		// Find the closest zombie to attack, if within range
		std::vector<EntitySystem::Entity> zombies = entity.getManager().getTaggedEntities("zombie");

		float closestDistance = FLT_MAX;
		unsigned int closest = 0;

		for (unsigned int i = 0; i < zombies.size(); ++i)
		{
			// Get distance to zombie
			glm::vec3 directionToZombie = zombies[i].getComponent<Engine::Transform>()->position - transform->positionWorld();
			float distanceToZombie = glm::length(directionToZombie);

			// Is this zombie closer?
			if (distanceToZombie < closestDistance)
			{
				closestDistance = distanceToZombie;
				closest = i;
			}
		}

		// Is the closest in range to attack?
		if (closestDistance <= 3.0f)
		{
			// Attack
			attackTimer = 0.6f;
			zombies[closest].getComponent<EnemyHealth>()->addDamage(5.0f);

			// Stop sound if already playing
			if (attackSound->isPlaying())
				attackSound->stop();

			// Play sound
			attackSound->play();
		}
	}

	attackTimer -= time->getDeltaTime();
}