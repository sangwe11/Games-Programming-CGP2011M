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

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();
	engine.setup("Zombie Survival", 800, 600, false, true);

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntity();
	player.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	player.addComponent<Engine::Camera>(60.0f, 0.3f, 1000.0f, true, false, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	player.addComponent<Engine::Skybox>("textures/Skyboxes/ame_starfield", "starfield_rt.tga", "starfield_lf.tga", "starfield_up.tga", "starfield_dn.tga", "starfield_bk.tga", "starfield_ft.tga");

	// Create crate entity
	EntitySystem::Entity crate = engine.entities.createEntity();
	crate.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, AI_DEG_TO_RAD(45.0f), 0.0f), glm::vec3(1.0f));
	crate.addComponent<Engine::MeshRenderer>("models/Environment/Crate/crate.obj");

	// Create sun entity
	//EntitySystem::Entity sun = engine.entities.createEntity();
	//sun.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//sun.addComponent<Engine::DirectionalLight>(glm::vec3(1.0f), 0.2f, 0.1f, glm::vec3(0.0f, -1.0f, -1.0f));

	// Create light
	EntitySystem::Entity light = engine.entities.createEntity();
	light.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	light.addComponent<Engine::SpotLight>(glm::vec3(1.0f), 1.0f, 0.01f, 20.0f, 1.0f, glm::vec3(0.0f, 0.0f, -1.0f), 20.0f, 50.0f);

	// Get a handle to the input system
	Engine::Input::Handle input = engine.systems.getSystem<Engine::Input>();

	// Get input devices
	Engine::KeyboardMouse &keyboardMouse = input->get<Engine::KeyboardMouse>(0);
	Engine::Controller &controller = input->get<Engine::Controller>(0);

	while (!input->checkQuit() && !keyboardMouse.getButtonDown("back") && !controller.getButtonDown("back"))
	{
		// Update input
		engine.update();

		// Print to console based on input
		if (keyboardMouse.getButtonDown("jump") || controller.getButtonDown("jump"))
			std::cout << "jump around!" << std::endl;
	}

	engine.cleanup();

	return 0;
}