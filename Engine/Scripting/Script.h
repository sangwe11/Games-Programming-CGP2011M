#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "../EntitySystem/Entity.h"
#include "../EntitySystem/World.h"
#include "Scripting.h"

namespace Engine
{
	template <typename Derived>
	class Script : public EntitySystem::BaseComponent
	{
	public:
		virtual ~Script() { }

		typedef EntitySystem::ComponentHandle<Derived> Handle;

		// Mask the initialise function with onAwake
		virtual void initialise() final;

		// Called ONCE when the component is created
		virtual void onAwake() {}

		// Called BEFORE the rendering calls ONCE per frame
		virtual void earlyUpdate() {}

		// Called AFTER the rendering calls ONCE per frame
		virtual void update() {}

		// Called AFTER update ONCE per frame
		virtual void lateUpdate() {}

		// Called once per physics update BEFORE the simulation
		virtual void fixedUpdate() {}

		// Mask the uninitialise function with onDestroy
		virtual void uninitialise() final;

		// Called ONCE when the component is destroyed
		virtual void onDestroy() {}

		static EntitySystem::TypeId getTypeId()
		{
			return EntitySystem::Type<EntitySystem::BaseComponent>::getTypeId<Derived>();
		}
	};

	template <typename Derived>
	void Script<Derived>::initialise()
	{
		// Add a system to handle the derived type if it doesn't exist
		if (!manager->getWorld().systems.systemExists<Scripting<Derived>>())
			manager->getWorld().systems.addSystem<Scripting<Derived>>();

		// Call onAwake
		onAwake();
	}

	template <typename Derived>
	void Script<Derived>::uninitialise()
	{
		// Call onDestroy
		onDestroy();
	}
}

#endif