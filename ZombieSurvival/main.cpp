#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Core/Instance.h>
#include <Core/Files.h>
#include <Core/Input.h>
#include <Core/KeyboardMouse.h>
#include <Core/Controller.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/Light.h>
#include <Rendering/BillboardRenderer.h>

#include "templates/Player.h"
#include "templates/Campfire.h"
#include "templates/Zombie.h"
#include "templates/SpawnPoints.h"
#include "templates/MedicalPack.h"
#include "scripts/spawning/ZombieSpawning.h"

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();
	engine.setup("Zombie Survival", 1400, 900, false, true);

	// Ambient music
	irrklang::ISound *ambiance =  engine.systems.getSystem<Engine::Audio>()->getAudio()->play2D("sounds/hoard.mp3", true, false, true);
	ambiance->setVolume(0.02f);
	ambiance->drop();

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntityFromTemplate(Player(glm::vec3(0.0f, 0.0f, 4.0f)));

	// Billboard forest
	EntitySystem::Entity forest = engine.entities.createEntity();
	forest.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0));
	Engine::BillboardRenderer::Handle billboard = forest.addComponent<Engine::BillboardRenderer>("textures/Billboards/tree.png", glm::vec2(4.0f, 5.5f));

	for (unsigned int distance = 20; distance < 100; distance += 5)
	{
		for (unsigned int i = 0; i < 20; ++i)
		{
			float randomOffset = (rand() % 720) / 2.0f;

			glm::vec3 forward = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), Engine::DegToRads(36.0f * i + randomOffset), glm::vec3(0.0f, 1.0f, 0.0));

			glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f) + forward * (float)distance;

			billboard->addBillboard(position);
		}
	}

	// Medical kit
	EntitySystem::Entity medical = engine.entities.createEntityFromTemplate(MedicalPack(glm::vec3(-5.0f, 0.0f, -5.0f)));


	// Ground
	EntitySystem::Entity ground = engine.entities.createEntity();
	ground.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.0f));
	ground.addComponent<Engine::MeshRenderer>("models/Environment/Ground/Ground.obj");

	// Camp fire
	EntitySystem::Entity campFire = engine.entities.createEntityFromTemplate(Campfire(glm::vec3(0.0f, 0.0f, 0.0f)));

	// Zombie spawning
	EntitySystem::Entity spawnpoints = engine.entities.createEntityFromTemplate(SpawnPoints(glm::vec3(0.0f, 0.0f, 0.0f)));
	EntitySystem::Entity zombieManager = engine.entities.createEntity();
	zombieManager.addComponent<ZombieSpawning>(10.0f, 10, 1.5f);

	// Ambient lighting
	EntitySystem::Entity sun = engine.entities.createEntity();
	sun.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	sun.addComponent<Engine::DirectionalLight>(glm::vec3(1.0f), 0.0f, 0.01f, glm::vec3(0.0f, -1.0f, -1.0f));

	// Get a handle to the input system
	Engine::Input::Handle input = engine.systems.getSystem<Engine::Input>();

	// Timing
	float deltaTime = 0.0f;
	unsigned int lastTime = 0;

	// Quit
	bool quit = false;

	while (!quit)
	{
		// Check for quit event
		if (input->checkQuit())
			quit = true;

		// Update
		engine.update();
	}

	engine.cleanup();

	return 0;
}