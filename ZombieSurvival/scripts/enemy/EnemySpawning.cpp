#include "EnemySpawning.h"

#include "../../Templates/EnemyTemplate.h"

EnemySpawning::EnemySpawning(const unsigned int &firstRoundCount, const unsigned int &firstRoundMax, const float &difficulty) : firstRoundCount(firstRoundCount), firstRoundMax(firstRoundMax), difficulty(difficulty)
{
}

void EnemySpawning::onAwake()
{
	// Tag entity
	entity.tag("enemyspawn");

	// Get required systems
	time = entity.getManager().getWorld().systems.getSystem<Engine::Time>();

	// Add components
	roundSound = entity.addComponent<Engine::AudioSource2D>("sounds/next_round.mp3", 50, false, false);
	currentRoundUI = entity.addComponent<Engine::UILabel>("", "fonts/Vera.ttf", 100, glm::vec4(1.0f), glm::vec2(-0.95f, -0.9f));
}

void EnemySpawning::update()
{
	// Start game
	if (currentRound == 0)
		nextRound();

	// Increment round time
	roundTime += time->getDeltaTime();

	// Update current round text
	currentRoundUI->setText(std::to_string(currentRound));

	// Make the round text red for the beginning of each new round
	if (roundTime <= 4.0f)
		currentRoundUI->setFontColor(glm::mix(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), roundTime / 4.0f));
	else
		currentRoundUI->setFontColor(glm::vec4(1.0f));

	std::cout << entity.getManager().getTaggedEntities("zombie").size() << std::endl;

	// Spawn zombies if currently alive is not at the maximum
	if ((entity.getManager().getTaggedEntities("zombie").size() < max) && (left > 0))
	{
		// Pick a random spawn point
		std::vector<EntitySystem::Entity> spawnPoints = entity.getManager().getTaggedEntities("spawnpoint");

		if (spawnPoints.size() <= 0)
		{
			std::cout << "No spawn points found!" << std::endl;
		}
		else
		{
			// Choose a random spawn point
			unsigned int spawn = rand() % spawnPoints.size();
			glm::vec3 position = spawnPoints[spawn].getComponent<Engine::Transform>()->position;

			// Spawn
			EntitySystem::Entity enemy = entity.getManager().createEntityFromTemplate(EnemyTemplate(position));
			--left;
		}
	}

	// Next round once all the enemies have spawned and been killed from this round
	if ((left <= 0) && (entity.getManager().getTaggedEntities("zombie").size() <= 0))
	{
		nextRound();
	}
}

void EnemySpawning::nextRound()
{
	// Increment round
	++currentRound;

	// Play sound effect
	roundSound->play();

	// Reset for next round
	roundTime = 0.0f;
	max = (int)((firstRoundMax * (difficulty * currentRound)) / difficulty);
	left = (int)((firstRoundCount * (difficulty * currentRound)) / difficulty);
}
