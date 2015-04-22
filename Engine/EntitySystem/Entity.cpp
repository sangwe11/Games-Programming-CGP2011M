#include "Entity.h"

#include <assert.h>
#include <algorithm>

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

	void Entity::setParent(Entity &entity)
	{
		manager->setParent(id, entity.id);
	}

	void Entity::removeParent()
	{
		manager->removeParent(id);
	}

	Entity Entity::addChild()
	{
		return manager->addChild(id);
	}

	void Entity::addChild(Entity &entity)
	{
		manager->addChild(id, entity.id);
	}

	void Entity::removeChild(Entity &entity)
	{
		manager->removeChild(id, entity.id);
	}

	Entity Entity::getParent()
	{
		return manager->getParent(id);
	}

	std::vector<Entity> Entity::getChildren()
	{
		return manager->getChildren(id);
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

	Entity EntityManager::createEntityFromTemplate(EntityTemplate &entityTemplate)
	{
		// Create a new entity
		Entity entity = createEntity();

		// Setup using template
		entityTemplate.build(entity, *this);
		return entity;
	}

	void EntityManager::setParent(Entity::Id entity, Entity::Id parent)
	{
		// Check for existing parent
		if (parents[entity] != Entity::invalidId)
		{
			// Remove from current parent child list
			removeChild(parents[entity], entity);
		}

		// Set new parent
		parents[entity] = parent;

		// Add child to parents children
		children[parent].push_back(entity);
	}

	void EntityManager::removeParent(Entity::Id entity)
	{
		// Check for existing parent
		if (parents[entity] != Entity::invalidId)
		{
			// Remove from current parent child list
			removeChild(parents[entity], entity);
		}

		parents[entity] = Entity::invalidId;
	}

	Entity EntityManager::addChild(Entity::Id entity)
	{
		// Create new entity
		Entity child = createEntity();

		// Parent e to entity
		setParent(child.getId(), entity);

		return child;
	}

	void EntityManager::addChild(Entity::Id entity, Entity::Id child)
	{
		// Parent child to entity
		setParent(child, entity);
	}

	void EntityManager::removeChild(Entity::Id entity, Entity::Id child)
	{
		// Remove child from entity
		children[entity].erase(std::remove(children[entity].begin(), children[entity].end(), child), children[entity].end());

		// Remove childs parent
		parents[child] = Entity::invalidId;
	}

	Entity EntityManager::getParent(Entity::Id entity)
	{
		return Entity(parents[entity], this);
	}

	std::vector<Entity> EntityManager::getChildren(Entity::Id entity)
	{
		std::vector<Entity> found;

		// Convert Entity::Id list into Entity list
		for (Entity::Id &id : children[entity])
			found.emplace_back(id, this);

		return found;
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