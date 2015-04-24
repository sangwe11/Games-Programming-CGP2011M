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

	void Entity::tag(const Entity::Tag &tag)
	{
		manager->tagEntity(id, tag);
	}

	bool Entity::tagged(const Entity::Tag &tag)
	{
		return manager->entityIsTagged(id, tag);
	}

	void Entity::removeAllComponents()
	{
		manager->removeAllComponentsFromEntity(id);
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

	void EntityManager::tagEntity(Entity::Id entity, const Entity::Tag &tag)
	{
		if (!entityIsTagged(entity, tag))
			tags[tag].push_back(entity);
	}

	void EntityManager::tagEntity(Entity::Id entity, const Entity::Tags &tags)
	{
		for (const Entity::Tag &tag : tags)
			if (!entityIsTagged(entity, tag))
				this->tags[tag].push_back(entity);
	}

	const bool EntityManager::entityIsTagged(Entity::Id entity, const Entity::Tag &tag)
	{
		if (std::find(tags[tag].begin(), tags[tag].end(), entity) != tags[tag].end())
			return true;
		else
			return false;
	}

	const bool EntityManager::entityIsTagged(Entity::Id entity, const Entity::Tags &tags)
	{
		for (const Entity::Tag &tag : tags)
		{
			if (std::find(this->tags[tag].begin(), this->tags[tag].end(), entity) == this->tags[tag].end())
				return false;
		}

		return true;
	}

	Entity EntityManager::getTaggedEntity(Entity::Tag tag)
	{
		if (tags[tag].size() > 0)
			return Entity(tags[tag][0], this);
		else
			return Entity();
	}

	std::vector<Entity> EntityManager::getTaggedEntities(Entity::Tag tag)
	{
		std::vector<Entity> found;

		for (Entity::Id &id : tags[tag])
			found.emplace_back(id, this);

		return found;
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

		// Remove parent
		removeParent(id);

		// Destroy children
		destroyEntities(children[id]);

		// Erase entity from parent / child
		parents.erase(id);
		children.erase(id);

		// Remove all components from entity
		removeAllComponentsFromEntity(id);

		// Delete all tags
		for (auto &tagList : tags)
			tagList.second.erase(std::remove(tagList.second.begin(), tagList.second.end(), id), tagList.second.end());
	}

	void EntityManager::destroyEntities(std::vector<Entity::Id> entities)
	{
		for (auto &entity : entities)
			destroyEntity(entity);
	}

	Entity EntityManager::getEntity(Entity::Id id)
	{
		assert(valid(id));
		return Entity(id, this);
	}

	void EntityManager::removeAllComponentsFromEntity(Entity::Id id)
	{
		// Uninit all components and remove
		for (auto &pair : components)
		{
			if (pair.second.size() > id.index())
			{
				if (pair.second[id.index()] != nullptr)
				{
					pair.second[id.index()]->uninitialise();
					pair.second[id.index()] = nullptr;
				}
			}
		}
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