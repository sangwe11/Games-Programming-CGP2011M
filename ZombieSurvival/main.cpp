#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Core/Instance.h>
#include <Core/KeyboardMouse.h>
#include <Core/Controller.h>

#include "Templates/PlayerTemplate.h"
#include "Templates/EnvironmentTemplate.h"
#include "Templates/SpawnPointsTemplate.h"

#include "Scripts/Controllers/GameControl.h"

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();
	engine.setup("Zombie Survival", 1400, 900, false, true);

	// Create environment from template
	EntitySystem::Entity environment = engine.entities.createEntityFromTemplate(EnvironmentTemplate());

	// Create player entity from template
	EntitySystem::Entity player = engine.entities.createEntityFromTemplate(PlayerTemplate(glm::vec3(0.0f, 0.0f, 15.0f)));

	// Enemy spawning
	EntitySystem::Entity enemySpawn = engine.entities.createEntity();
	enemySpawn.addComponent<EnemySpawning>(10, 5, 1.5f);

	// Spawn points
	EntitySystem::Entity spawnPoints = engine.entities.createEntityFromTemplate(SpawnPointsTemplate(glm::vec3(0.0f, 0.0f, 0.0f)));

	// Game controller
	EntitySystem::Entity gameControl = engine.entities.createEntity();
	gameControl.addComponent<GameControl>();

	bool quit = false;

	do
	{
		quit = engine.systems.getSystem<Engine::Input>()->checkQuit();

		engine.update();

	} while (!quit);

	engine.cleanup();

	return 0;
}