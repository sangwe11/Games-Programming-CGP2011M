#include "Entity.h"

#include <assert.h>

namespace EntitySystem
{
	const Entity::Id Entity::invalidId;

	bool Entity::valid() const
	{
		return manager && manager->valid(id);
	}

	void Entity::invalidate()
	{
		id = invalidId;
		manager = nullptr;
	}

	void Entity::destroy()
	{
		assert(valid());
		manager->destroyEntity(id);
		invalidate();
	}

	EntityManager::~EntityManager()
	{
		reset();
	}

	Entity EntityManager::createEntity()
	{
		uint32_t index, version;

		// Reuse entity index if one is available
		if (!free.empty())
		{
			// Use the first free index
			index = free.back();
			free.pop_back();

			// Set version
			version = versions[index];
		}
		else
		{
			// Use the next index
			index = nextIndex++;

			// Resize storage if needed
			ensureSize(index);

			// Set version = 1
			version = versions[index] = 1;
		}

		// Create Entity
		Entity entity(Entity::Id(index, version), this);

		return entity;
	}

	void EntityManager::destroyEntity(Entity::Id id)
	{
		assert(valid(id));

		// Increment version to invalidate id
		++versions[id.index()];

		// Add index to free list
		free.push_back(id.index());

		// Delete all components
		for (auto &pair : components)
			pair.second[id.index()] = nullptr;
	}

	Entity EntityManager::getEntity(Entity::Id id)
	{
		assert(valid(id));
		return Entity(id, this);
	}

	void EntityManager::resize(size_t size)
	{
		// Resize version list
		versions.resize(size);

		// Resize component lists
		for (auto &pair : components)
			pair.second.resize(size);
	}

	void EntityManager::reset()
	{
		// Clear version list
		versions.clear();

		// Clear free list
		free.clear();

		// Reset next index
		nextIndex = 0;
	}

	void EntityManager::ensureSize(std::uint32_t index)
	{
		// Resize if needed
		if (versions.size() <= index)
			resize(index + 1);
	}

}