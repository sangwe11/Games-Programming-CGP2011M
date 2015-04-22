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
	player.addComponent<Engine::Transform>(glm::vec3(0.0f, 2.4f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	player.addComponent<Engine::Camera>(60.0f, 0.3f, 1000.0f, true, false, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	player.addComponent<Engine::Skybox>("textures/Skyboxes/ame_starfield", "starfield_rt.tga", "starfield_lf.tga", "starfield_up.tga", "starfield_dn.tga", "starfield_bk.tga", "starfield_ft.tga");
	
	EntitySystem::Entity spotlight = engine.entities.createEntity();
	spotlight.addComponent<Engine::Transform>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));	
	player.addComponent<Engine::SpotLight>(glm::vec3(1.0f), 1.0f, 0.0f, 20.0f, glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, 20.0f);

	// Ground
	EntitySystem::Entity ground = engine.entities.createEntity();
	ground.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	ground.addComponent<Engine::MeshRenderer>("models/Environment/Ground/Ground.obj");

	// Create crate entity
	EntitySystem::Entity crate = engine.entities.createEntity();
	crate.addComponent<Engine::Transform>(glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.0f, AI_DEG_TO_RAD(45.0f), 0.0f), glm::vec3(1.0f));
	crate.addComponent<Engine::MeshRenderer>("models/Environment/Crate/crate.obj");

	// Create sun entity
	//EntitySystem::Entity sun = engine.entities.createEntity();
	//sun.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//sun.addComponent<Engine::DirectionalLight>(glm::vec3(1.0f), 0.01f, 0.01f, glm::vec3(0.0f, -1.0f, -1.0f));

	// Get a handle to the input system
	Engine::Input::Handle input = engine.systems.getSystem<Engine::Input>();

	// Timing
	float deltaTime = 0.0f;
	unsigned int lastTime = 0;

	// Quit
	bool quit = false;

	while (!quit)
	{
		glm::vec2 move;
		glm::vec2 look;
		bool sprint;
		bool jump;

		// Update
		engine.update();

		if (input->exists<Engine::Controller>(0))
		{
			Engine::Controller &c = input->get<Engine::Controller>(0);

			// Use controller if present
			move.x = c.getAxis("moveX");
			move.y = c.getAxis("moveY");
			look.x = c.getAxis("lookX");
			look.y = c.getAxis("lookY");
			sprint = c.getButton("sprint");
			jump = c.getButtonDown("jump");
			quit = c.getButton("back");
		}
		else
		{
			Engine::KeyboardMouse k = input->get<Engine::KeyboardMouse>(0);

			// Fallback to keyboard and mouse
			move.x = k.getAxis("moveX");
			move.y = k.getAxis("moveY");
			look.x = k.getAxis("lookX");
			look.y = k.getAxis("lookY");
			sprint = k.getButton("sprint");
			jump = k.getButtonDown("jump");
			quit = k.getButton("back");
		}

		// Check for quit event
		if (input->checkQuit())
			quit = true;

		// Some basic player movement
		Engine::Transform::Handle transform = player.getComponent<Engine::Transform>();

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
				transform->position += direction * 6.0f * deltaTime;
			else
				transform->position += direction * 4.0f * deltaTime;
		}

		// Rotate camera on look axis
		transform->rotateGlobal(look.x * -2.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		transform->rotateLocal(look.y * -2.0f * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));

		// Update delta time
		unsigned int currentTime = SDL_GetTicks();
		unsigned int frameTime = currentTime - lastTime;
		lastTime = currentTime;
		deltaTime = frameTime / 1000.0f;
	}

	engine.cleanup();

	return 0;
}