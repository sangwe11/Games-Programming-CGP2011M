#include "ZombieSpawning.h"
#include "../../templates/Zombie.h"
#include <Core/Time.h>
#include <EntitySystem/World.h>
#include <Rendering/Display.h>

ZombieSpawning::ZombieSpawning(const float &roundTime, const unsigned int &startCount, const float &roundMultiplier)
{
	this->roundTime = roundTime;
	this->startCount = startCount;
	this->roundMultiplier = roundMultiplier;
}

void ZombieSpawning::onAwake()
{
	// Add sound
	nextRoundSound = entity.addComponent<Engine::AudioSource2D>("sounds/next_round.mp3", 50, false, false);
	nextRoundText = entity.addComponent<Engine::UILabel>("", "fonts/Vera.ttf", 60, glm::vec4(1.0f), glm::vec2(-0.3f, 0.2f));

	// State
	currentRound = 0;
	leftToSpawn = startCount;
	spawnDelay = startCount / roundTime;
	roundSpawned = false;
	roundFinished = false;

	// Show round message
	newRoundTimer = 3.0f;
	messageShown = true;
	nextRoundText->setText("Next Wave: " + std::to_string(currentRound + 1));
}

void ZombieSpawning::update()
{
	float deltaTime = manager->getWorld().systems.getSystem<Engine::Time>()->getDeltaTime();

	// Update timers
	spawnTimer -= deltaTime;

	// Check whether to hide GUI message
	if (messageShown)
	{
		if (newRoundTimer <= 0)
		{
			// Hide message
			nextRoundText->disable();
			messageShown = false;
		}
		else
		{
			newRoundTimer -= deltaTime;
		}
	}

	if (leftToSpawn > 0)
	{
		if (spawnTimer <= 0.0f)
		{
			// Reset timer
			spawnTimer = spawnDelay;

			// Get the spawn points
			std::vector<EntitySystem::Entity> spawnPoints = manager->getTaggedEntity("spawn_points").getChildren();

			if (spawnPoints.size() > 0)
			{
				// Spawn position
				unsigned int spawn = rand() % spawnPoints.size();
				glm::vec3 position = spawnPoints[spawn].getComponent<Engine::Transform>()->position;

				// Spawn new zombie
				EntitySystem::Entity zombie = manager->createEntityFromTemplate(Zombie(position));

				// One less left to spawn
				--leftToSpawn;
			}
			else
			{
				std::cout << "No spawn points found." << std::endl;
			}
		}
	}
	else if ((leftToSpawn <= 0) &&!roundSpawned)
	{
		roundSpawned = true;
	}
	else if (roundSpawned && !roundFinished)
	{
		std::vector<EntitySystem::Entity> zombies = entity.getManager().getTaggedEntities("zombie");

		if (zombies.size() <= 0)
		{
			roundFinished = true;
		}
	}
	else if (roundFinished)
	{
		nextRound();
	}
}

void ZombieSpawning::nextRound()
{
	// Increment round counter
	++currentRound;

	// Show round message for a short while
	nextRoundText->enable();
	nextRoundText->setText("Next Wave: " + std::to_string(currentRound + 1));

	// Play sound effect
	nextRoundSound->play();

	// Reset left to spawn
	leftToSpawn = (int)(startCount * (roundMultiplier * currentRound));
	spawnDelay = leftToSpawn / roundTime;
	roundSpawned = false;
	roundFinished = false;
	newRoundTimer = 3.0f;
	messageShown = true;
}