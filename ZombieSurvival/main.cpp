#include <Core/Instance.h>
#include <Core/Files.h>
#include <Core/Input.h>
#include <Core/KeyboardMouse.h>

int main(int argc, char *argv[])
{
	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();

	// Add a file system and store a handle to it
	Engine::Files::Handle files = engine.systems.addSystem<Engine::Files>();

	// Add an input system and store a handle to it
	Engine::Input::Handle input = engine.systems.addSystem<Engine::Input>();

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntity();

	while (!input->checkQuit())
	{
		// Update input
		engine.update();
	}

	return 0;
}