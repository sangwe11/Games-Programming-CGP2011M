#include <Core/Instance.h>
#include <Core/Files.h>
#include <Core/Input.h>
#include <Core/KeyboardMouse.h>
#include <Core/Controller.h>

int main(int argc, char *argv[])
{
	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();
	engine.setup("Zombie Survival", 800, 600, false, true);

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntity();

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

	return 0;
}