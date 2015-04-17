#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <map>
#include <memory>
#include <assert.h>

#include "TypeId.h"
#include "Entity.h"

namespace EntitySystem
{
	class World;
	class SystemManager;

	template <typename System>
	class SystemHandle;

	class BaseSystem
	{
	public:
		virtual ~BaseSystem() { }

		// Called when system is added to a SystemManager
		virtual void initialise() { }

		// Called when system is removed from a SystemManager
		virtual void uninitialise() { }

		// Modify components
		virtual void update(EntityManager &entities) = 0;

		// Get manager system belongs to
		SystemManager &getManager()
		{
			return *manager;
		}

	protected:
		friend class SystemManager;
		unsigned int priority = 50;

		SystemManager *manager;
	};

	template <typename Derived>
	class System : public BaseSystem
	{
	public:
		typedef SystemHandle<Derived> Handle;

		virtual ~System() { }

		static TypeId getTypeId()
		{
			return Type<BaseSystem>::getTypeId<Derived>();
		}
	};

	class SystemManager
	{
	public:
		SystemManager(World &world) : world(world) { }

		template <typename System, typename ... Args>
		typename System::Handle addSystem(Args && ... args)
		{
			TypeId type = Type<BaseSystem>::getTypeId<System>();

			// Ensure there is enough storage space
			ensureSize(type);

			systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
			systems[type]->manager = this;
			systems[type]->initialise();

			return SystemHandle<System>(this);
		}

		template <typename System>
		typename System::Handle getSystem()
		{
			assert(systemExists<System>());			
			return SystemHandle<System>(this);
		}

		template <typename System>
		bool systemExists() const
		{
			TypeId type = Type<BaseSystem>::getTypeId<System>();
			return type < systems.size() && systems[type] != nullptr;
		}

		template <typename System>
		void removeSystem()
		{
			assert(systemExists<System>());

			TypeId type = Type<BaseSystem>::getTypeId<System>();

			systems[type]->uninitialise();
			systems[type] = nullptr;
		}

		void updateAll();

		// Get the world the manager belongs to
		World &getWorld()
		{
			return world;
		}

	private:
		template <typename System>
		friend class SystemHandle;

		template <typename System>
		System &get()
		{
			return dynamic_cast<System &>(*systems[Type<BaseSystem>::getTypeId<System>()]);
		}

		void ensureSize(size_t index)
		{
			if (systems.size() <= index)
				systems.resize(index + 1);
		}

		std::vector<std::unique_ptr<BaseSystem>> systems;
		World &world;
	};

	template <typename System>
	class SystemHandle
	{
	public:

		SystemHandle() : manager(nullptr) { }
		SystemHandle(SystemManager *manager) : manager(manager) { }

		bool valid() const
		{
			return manager && manager->systemExists<System>();
		}

		operator bool() const
		{
			return valid();
		}

		System *operator->()
		{
			assert(valid());
			return &manager->get<System>();
		}

		const System *operator->() const
		{
			assert(valid());
			return &manager->get<System>();
		}

		System &get()
		{
			assert(valid());
			return manager->get<System>();
		}

		const System &get() const
		{
			assert(valid());
			return manager->get<System>();
		}

		void remove()
		{
			assert(valid());
			return manager->removeSystem<System>();
		}

		bool operator==(const SystemHandle<System> &other) const
		{
			assert(valid());
			return other.manager == manager;
		}

		bool operator!=(const SystemHandle<System> &other) const
		{
			return !(other == *this);
		}

	private:
		SystemManager *manager;
	};


}
#endif