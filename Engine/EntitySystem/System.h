#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <map>
#include <memory>
#include <assert.h>
#include <functional>

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

		// Add function to main update loop
		template <typename T>
		void addUpdateFunction(void(T::*function)(), T &system, const int &priority, const bool &fixed = false)
		{
			// Bind function
			std::function<void()> call = std::bind(function, &system);

			// Add function
			manager->addUpdateFunction(T::getTypeId(), call, priority, fixed);
		}

		// Get manager system belongs to
		SystemManager &getManager()
		{
			return *manager;
		}

	protected:
		friend class SystemManager;

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

		// Add functions to update/fixed update loop
		void addUpdateFunction(const TypeId &typeId, std::function<void()> function, const int &priority, const bool &fixed);

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
			// Assert system exists
			assert(systemExists<System>());

			// Get system type
			TypeId type = Type<BaseSystem>::getTypeId<System>();

			// Remove system
			systems[type]->uninitialise();
			systems[type] = nullptr;

			// Remove update functions
			std::multimap<int, std::pair<TypeId, std::function<void()>>>::iterator iter = updateFunctions.begin();

			while (iter != updateFunctions.end())
			{
				if (iter->second.first == type)
					iter = updateFunctions.erase(iter);
				else
					++iter;
			}

			// Remove fixed update functions
			std::multimap<int, std::pair<TypeId, std::function<void()>>>::iterator iter2 = fixedUpdateFunctions.begin();

			while (iter2 != fixedUpdateFunctions.end())
			{
				if (iter2->second.first == type)
					iter2 = fixedUpdateFunctions.erase(iter2);
				else
					++iter2;
			}

		}

		void removeAll();

		void update();
		void fixedUpdate();

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
		std::multimap<int, std::pair<TypeId, std::function<void()>>> updateFunctions;
		std::multimap<int, std::pair<TypeId, std::function<void()>>> fixedUpdateFunctions;
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