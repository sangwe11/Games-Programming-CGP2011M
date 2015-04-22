#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Core/Instance.h>
#include <Core/Files.h>
#include <Core/Input.h>
#include <Core/KeyboardMouse.h>
#include <Core/Controller.h>
#include <Rendering/Model.h>
#include <Rendering/Camera.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/Skybox.h>
#include <Rendering/Light.h>
#include <Rendering/UILabel.h>
#include <Audio/AudioListener.h>
#include <Audio/AudioSource.h>

#include "scripts/PlayerController.h"
#include "scripts/enemy/zombie.h"

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();
	engine.setup("Zombie Survival", 1400, 900, false, true);

	// Ambient music
	irrklang::ISound *ambiance =  engine.systems.getSystem<Engine::Audio>()->getAudio()->play2D("sounds/hoard.mp3", true, false, true);
	ambiance->setVolume(0.005f);
	ambiance->drop();

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntity();
	player.addComponent<Engine::Transform>(glm::vec3(0.0f, 2.4f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	player.addComponent<Engine::Camera>(60.0f, 0.3f, 1000.0f, false, false, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	player.addComponent<Engine::Skybox>("textures/Skyboxes/ame_starfield", "starfield_rt.tga", "starfield_lf.tga", "starfield_up.tga", "starfield_dn.tga", "starfield_bk.tga", "starfield_ft.tga");
	player.addComponent<Engine::UILabel>("Health: 100%", "fonts/Vera.ttf", 22, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(-0.97f, 0.92f));
	player.addComponent<Engine::PlayerController>();

	// Ground
	EntitySystem::Entity ground = engine.entities.createEntity();
	ground.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//ground.addComponent<Engine::MeshRenderer>("models/Environment/Ground/Ground.obj");

	// Create crate entity
	EntitySystem::Entity crate = engine.entities.createEntity();
	crate.addComponent<Engine::Transform>(glm::vec3(0.0f, -12.0f, 0.0f), glm::vec3(0.0f, AI_DEG_TO_RAD(45.0f), 0.0f), glm::vec3(0.2f));
	crate.addComponent<Engine::MeshRenderer>("models/Environment/old fashion town/old town block.obj");
	//crate.addComponent<Engine::AudioSource>("sounds/poltergeist.wav", 40, true, true)->play();

	// Create sun entity
	EntitySystem::Entity sun = engine.entities.createEntity();
	sun.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	sun.addComponent<Engine::DirectionalLight>(glm::vec3(1.0f), 0.0f, 0.0f, glm::vec3(0.0f, -1.0f, -1.0f));

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