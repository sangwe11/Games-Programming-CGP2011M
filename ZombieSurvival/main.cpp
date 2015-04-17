#include <Core/Instance.h>
#include <Core/Files.h>

int main(int argc, char *argv[])
{
	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();

	// Add a file system and get a handle to the added system
	Engine::Files::Handle files = engine.systems.addSystem<Engine::Files>();

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntity();

	return 0;
}