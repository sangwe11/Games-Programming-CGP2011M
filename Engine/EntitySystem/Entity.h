#ifndef _ENTITY_H
#define _ENTITY_H

#include <iostream>

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <memory>

#include "TypeId.h"

namespace EntitySystem
{
	class World;
	class EntityManager;

	template <typename Component>
	class ComponentHandle;

	class Entity
	{
	public:
		struct Id
		{
			Id() : id(0) { }
			explicit Id(std::uint64_t id) : id(id) { }
			Id(std::uint32_t index, std::uint32_t version) : id(std::uint64_t(index) | uint64_t(version) << 32UL) { }

			uint64_t get() const { return id; }

			bool operator== (const Id &other) const { return id == other.id; }
			bool operator!= (const Id &other) const { return id != other.id; }
			bool operator< (const Id &other) const { return id < other.id; }

			uint32_t index() const { return id & 0xffffffffUL; }
			uint32_t version() const { return id >> 32; }

		private:
			std::uint64_t id;
		};

		// Invalid entity id
		static const Id invalidId;

		// Default constructors
		Entity() = default;
		Entity(const Entity &other) = default;
		Entity &operator= (const Entity &other) = default;

		// Construct entity from Id and EntityManager *
		Entity(Id id, EntityManager *manager) : id(id), manager(manager) { }

		// Wrapped around valid()
		operator bool() const
		{
			return valid();
		}

		// Comparison operators
		bool operator==(const Entity &other) const
		{
			return other.manager == manager && other.id == id;
		}

		bool operator!=(const Entity &other) const
		{
			return !(other == *this);
		}

		bool operator<(const Entity &other) const
		{
			return other.id < id;
		}

		// Check if the entity is valid
		bool valid() const;

		// Invalidate an entity
		void invalidate();

		// Get entity id
		Id getId() const { return id; }

		template <typename Component, typename ... Args>
		typename Component::Handle addComponent(Args && ... args);

		template <typename Component>
		void removeComponent();

		template <typename Component>
		typename Component::Handle getComponent();

		template <typename Component>
		const typename Component::Handle getComponent() const;

		template <typename Component>
		bool hasComponent() const;

		// Destroy entity
		void destroy();

		// Get the manager the entity belongs to
		EntityManager &getManager()
		{
			return *manager;
		}

	private:
		EntityManager *manager = nullptr;
		Id id = invalidId;
	};

	template <typename Component>
	class ComponentHandle
	{
	public:

		ComponentHandle() : manager(nullptr) { }
		ComponentHandle(Entity::Id entity, EntityManager *manager) : entity(entity), manager(manager) { }

		bool valid() const;
		operator bool() const;
		Component *operator->();
		const Component *operator->() const;
		Component &get();
		const Component &get() const;
		Entity getEntity();
		void remove();

		bool operator==(const ComponentHandle<Component> &other) const
		{
			return other.manager == manager && other.entity == entity;
		}

		bool operator!=(const ComponentHandle<Component> &other) const
		{
			return !(other == *this);
		}

	private:
		EntityManager *manager;
		Entity::Id entity = Entity::invalidId;
	};

	struct BaseComponent
	{
	public:
		virtual ~BaseComponent() { }
		friend class EntityManager;

		virtual void initialise() { }
		virtual void uninitialise() { }

		bool isEnabled() const { return enabled; }
		void enable() { enabled = true; }
		void disable() { enabled = false; }

	protected:
		EntityManager *manager = nullptr;
		Entity entity;
		bool enabled = true;
	};

	template <typename Derived>
	struct Component : public BaseComponent
	{
	public:
		virtual ~Component() { }

		typedef ComponentHandle<Derived> Handle;

		static TypeId getTypeId()
		{
			return Type<BaseComponent>::getTypeId<Derived>();
		}
	};

	class EntityTemplate
	{
	public:
		virtual void build(Entity &entity, EntityManager &entities) = 0;
	};

	class EntityManager
	{
	public:
		EntityManager(World &world) : world(world) { }
		~EntityManager();

		size_t size() const { return nextIndex - free.size(); }
		size_t capacity() const { return nextIndex; }

		bool valid(Entity::Id id) const
		{
			return id.index() < versions.size() && versions.at(id.index()) == id.version();
		}

		// Create a new entity
		Entity createEntity();

		// Create a new entity from template class
		Entity createEntityFromTemplate(EntityTemplate &entityTemplate)
		{
			// Create a new entity
			Entity entity = createEntity();

			// Setup using template
			entityTemplate.build(entity, *this);
			return entity;
		}

		// Destroy an entity
		void destroyEntity(Entity::Id entity);

		// Get an entity by id
		Entity getEntity(Entity::Id id);

		template <typename Component, typename ... Args>
		typename Component::Handle addComponentToEntity(Entity::Id id, Args && ... args)
		{
			assert(valid(id));

			// Get component typeId
			TypeId type = Type<BaseComponent>::getTypeId<Component>();

			// Check component list exist, if not allocate enough space
			if (components[type].size() == 0)
				components[type].resize(size());

			// Create component
			components[type][id.index()] = std::make_unique<Component>(std::forward<Args>(args) ...);

			// Set manager pointer and entity id
			components[type][id.index()]->manager = this;
			components[type][id.index()]->entity = Entity(id, this);

			// Initialise
			components[type][id.index()]->initialise();

			// Return handle
			return ComponentHandle<Component>(id, this);
		}

		template <typename Component>
		typename Component::Handle getComponentFromEntity(Entity::Id id)
		{
			assert(valid(id) && entityHasComponent<Component>(id));
			return ComponentHandle<Component>(id, this);
		}

		template <typename Component>
		bool entityHasComponent(Entity::Id id) const
		{
			assert(valid(id));

			TypeId type = Type<BaseComponent>::getTypeId<Component>();

			auto iter = components.find(type);

			// Return false if component type list doesn't exist
			if (iter == components.end())
				return false;

			// Return false if component list too small
			if (iter->second.size() <= id.index())
				return false;

			// Return false if component doesn't exist
			if (iter->second[id.index()] == nullptr)
				return false;

			// Return true
			return true;
		}

		template <typename Component>
		void removeComponentFromEntity(Entity::Id id)
		{
			assert(valid(id) && entityHasComponent<Component>(id));
			components[Type<BaseComponent>::getTypeId<Component>()][id.index()] = nullptr;
		}

		template <typename Component>
		std::vector<typename Component::Handle> getAllComponents(const bool &enabledOnly)
		{
			std::vector<typename Component::Handle> found;

			// Get type and component counts
			TypeId type = Type<BaseComponent>::getTypeId<Component>();
			size_t count = components[type].size();

			// Reserve space in found list to save reallocations
			found.reserve(count);

			// Loop through component type and add to found if enabled (or if we don't care if the component is enabled)
			for (unsigned int i = 0; i < count; ++i)
				if (components[type][i] != nullptr)
					if ((enabledOnly && components[type][i]->isEnabled()) || !enabledOnly)
						found.emplace_back(components[type][i]->entity.getId(), components[type][i]->manager);

			return found;
		}

		template <typename Component>
		std::vector<typename Component::Handle> getAllDerivedComponents(const bool &enabledOnly)
		{
			std::vector<typename Component::Handle> found;

			// Get type and component counts
			TypeId type = Type<BaseComponent>::getTypeId<Component>();

			// Loop through component types
			for (auto &pair : components)
			{
				// Check if there are any components of this type
				if (pair.second.size() > 0)
				{
					// Is the component type derived from type Component?
					if (Component *result = dynamic_cast<Component *>(pair.second[0].get()))
					{
						// Loop through component type and add to found if enabled (or if we don't care if the component is enabled)
						for (unsigned int i = 0; i < pair.second.size(); ++i)
							if (pair.second[i] != nullptr)
								if ((enabledOnly && pair.second[i]->enabled) || !enabledOnly)
									found.emplace_back(pair.second[i]->entity.getId(), pair.second[i]->manager);
					}
				}
			}

			return found;
		}

		// Resize storage
		void resize(size_t size);

		// Get the world the manager belongs to
		World &getWorld()
		{
			return world;
		}

		// Destroy all entities and associated components
		void reset();

	private:
		template <typename Component>
		friend class ComponentHandle;

		template <typename Component>
		Component &getComponent(Entity::Id id)
		{
			//assert(valid(id));
			return dynamic_cast<Component &>(*components[Type<BaseComponent>::getTypeId<Component>()][id.index()]);
		}

		// Ensure the storage size can handle the entity index
		void ensureSize(std::uint32_t index);

		// Next entity index to use if there are no free indexes
		std::uint32_t nextIndex = 0;

		// Components are stored in a vector per component type for cache contiguous
		std::unordered_map <TypeId, std::vector<std::unique_ptr<BaseComponent>>> components;

		// List of entity versions
		std::vector<std::uint32_t> versions;

		// List of free entity indexes
		std::vector<std::uint32_t> free;

		// Reference to entity world
		World &world;
	};

	template <typename Component, typename ... Args>
	typename Component::Handle Entity::addComponent(Args && ... args)
	{
		//assert(valid());
		return manager->addComponentToEntity<Component>(id, std::forward<Args>(args) ...);
	}

	template <typename Component>
	void Entity::removeComponent()
	{
		//assert(valid() && hasComponent<Component>());
		manager->removeComponentFromEntity<Component>(id);
	}

	template <typename Component>
	typename Component::Handle Entity::getComponent()
	{
		//assert(valid() && hasComponent<Component>());
		return manager->getComponentFromEntity<Component>(id);
	}

	template <typename Component>
	const typename Component::Handle Entity::getComponent() const
	{
		//assert(valid() && hasComponent<Component>());
		return manager->getComponentFromEntity<const Component>(id);
	}

	template <typename Component>
	bool Entity::hasComponent() const
	{
		//assert(valid());
		return manager->entityHasComponent<Component>(id);
	}

	template <typename Component>
	bool ComponentHandle<Component>::valid() const
	{
		return manager && manager->valid(entity) && manager->entityHasComponent<Component>(entity);
	}

	template <typename Component>
	ComponentHandle<Component>::operator bool() const
	{
		return valid();
	}

	template <typename Component>
	Component *ComponentHandle<Component>::operator->()
	{
		assert(valid());
		return &manager->getComponent<Component>(entity);
	}

	template <typename Component>
	const Component *ComponentHandle<Component>::operator->() const
	{
		assert(valid());
		return &manager->getComponent<Component>(entity);
	}

	template <typename Component>
	Component &ComponentHandle<Component>::get()
	{
		assert(valid());
		return manager->getComponent<Component>(entity);
	}

	template <typename Component>
	const Component &ComponentHandle<Component>::get() const
	{
		assert(valid());
		return manager->getComponent<Component>(entity);
	}

	template <typename Component>
	Entity ComponentHandle<Component>::getEntity()
	{
		assert(valid());
		return manager->getEntity(entity);
	}

	template <typename Component>
	void ComponentHandle<Component>::remove()
	{
		assert(valid());
		return manager->removeComponentFromEntity<Component>(entity);
	}
}

#endif