#include "System.h"
#include "World.h"

#include <algorithm>

namespace EntitySystem
{
	void SystemManager::addUpdateFunction(const TypeId &typeId, std::function<void()> function, const int &priority, const bool &fixed)
	{
		if (fixed)
			fixedUpdateFunctions.insert(std::make_pair(priority, std::make_pair(typeId, function)));
		else
			updateFunctions.insert(std::make_pair(priority, std::make_pair(typeId, function)));
	}

	void SystemManager::update()
	{
		// Call each function in the update function list
		for (auto &function : updateFunctions)
			function.second.second();
	}

	void SystemManager::fixedUpdate()
	{
		// Call each function in the fixed update function list
		for (auto &function : fixedUpdateFunctions)
			function.second.second();
	}
}