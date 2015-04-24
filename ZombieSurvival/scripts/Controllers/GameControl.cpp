#include "GameControl.h"

#include <Core/Controller.h>
#include <Core/KeyboardMouse.h>
#include "../Enemy/EnemyHealth.h"

void GameControl::onAwake()
{
	// Tag entity
	entity.tag("gamecontrol");

	// Get required systems
	input = entity.getManager().getWorld().systems.getSystem<Engine::Input>();

	// Get player components
	movement = entity.getManager().getTaggedEntity("player").getComponent<PlayerMovement>();
	camera = entity.getManager().getTaggedEntity("player").getComponentInChildren<PlayerCameraControl>();
	health = entity.getManager().getTaggedEntity("player").getComponent<PlayerHealth>();
	accessories = entity.getManager().getTaggedEntity("player").getComponentInChildren<PlayerAccessories>();

	// Get spawning component
	spawning = entity.getManager().getTaggedEntity("enemyspawn").getComponent<EnemySpawning>();

	// Create UI components
	scoreUI = entity.addComponent<Engine::UILabel>("Score: ", "fonts/Vera.ttf", 25, glm::vec4(1.0f), glm::vec2(-0.2f, 0.92f));
	gamestartUI = entity.addChild().addComponent<Engine::UILabel>("Press [enter] or [start] to start!", "fonts/Vera.ttf", 50, glm::vec4(1.0f), glm::vec2(-0.55f, 0.1f));
	gameoverUI = entity.addChild().addComponent<Engine::UILabel>("Game Over!", "fonts/Vera.ttf", 80, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-0.35f, 0.1f));

	// Disable gameover UI
	gameoverUI->disable();

	// Disable the player
	movement->disable();
	camera->disable();
	accessories->disable();

	// Disable spawning
	spawning->disable();
}

void GameControl::update()
{
	// Check for game start
	if (!started)
	{
		bool start;

		if (input->exists<Engine::Controller>(0))
		{
			Engine::Controller &c = input->get<Engine::Controller>(0);

			// Use controller if present
			start = c.getButtonDown("start");
		}
		else
		{
			Engine::KeyboardMouse &k = input->get<Engine::KeyboardMouse>(0);

			// Fallback to keyboard and mouse
			start = k.getButtonDown("start");
		}

		if (start)
		{
			started = true;

			// Disable game start UI
			gamestartUI->disable();

			// Enable player
			movement->enable();
			camera->enable();
			accessories->enable();

			// Enable spawning
			spawning->enable();
		}
	}

	// Destroy any dead enemies and award score
	for (EnemyHealth::Handle &health : entity.getManager().getAllComponents<EnemyHealth>(false))
	{
		if (!health->alive())
		{
			health.getEntity().destroy();
			score += 20;
		}
	}

	// Check for game over
	if (!health->alive())
	{
		// Disable the player
		movement->disable();
		camera->disable();

		// Game over
		gameOver = true;

		// Display game over UI
		gameoverUI->enable();
	}

	// Handle exiting
	if (gameOver || !started)
	{
		if (input->exists<Engine::Controller>(0))
		{
			Engine::Controller &c = input->get<Engine::Controller>(0);

			// Use controller if present
			if (c.getButtonDown("start") || c.getButtonDown("quit"))
				input->triggerQuit();
		}
		else
		{
			Engine::KeyboardMouse &k = input->get<Engine::KeyboardMouse>(0);

			// Fallback to keyboard and mouse
			if (k.getButtonDown("start") || k.getButtonDown("quit"))
				input->triggerQuit();
		}
	}

	// Update score UI
	scoreUI->setText("Score: " + std::to_string(score));
}

void GameControl::addScore(const unsigned int &amount)
{
	score += amount;
}