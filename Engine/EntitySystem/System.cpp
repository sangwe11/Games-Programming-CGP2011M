#include "System.h"
#include "World.h"

#include <algorithm>

namespace EntitySystem
{
	void SystemManager::updateAll()
	{
		// Sort systems by priority
		std::sort(systems.begin(), systems.end(), [](std::unique_ptr<BaseSystem> &x, std::unique_ptr<BaseSystem> &y) { return x->priority < y->priority; });

		// Update each system in turn
		for (auto &system : systems)
			system->update(world.entities);
	}
}