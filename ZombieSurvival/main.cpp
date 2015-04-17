#include <Core/Instance.h>

int main(int argc, char *argv[])
{
	// Create a new instance of Engine
	Engine::Instance engine = Engine::Instance();

	// Create player entity
	EntitySystem::Entity player = engine.entities.createEntity();

	return 0;
}