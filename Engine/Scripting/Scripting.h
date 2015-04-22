#ifndef _SCRIPTING_H
#define _SCRIPTING_H

#include <vector>

#include "../EntitySystem/System.h"
#include "../EntitySystem/World.h"

namespace Engine
{
	template <typename Script>
	class Scripting : public EntitySystem::System<Scripting<Script>>
	{
	public:
		virtual void initialise()
		{
			// Add functions to update loop
			addUpdateFunction(&Scripting<Script>::earlyUpdate, *this, 15);
			addUpdateFunction(&Scripting<Script>::update, *this, 60);
			addUpdateFunction(&Scripting<Script>::lateUpdate, *this, 70);

			// Add functions to fixed update loop
			addUpdateFunction(&Scripting<Script>::fixedUpdate, *this, 50, true);
		}

		void earlyUpdate()
		{
			for (Script::Handle &script : manager->getWorld().entities.getAllDerivedComponents<Script>(true))
				script->earlyUpdate();
		}

		void update()
		{
			for (Script::Handle &script : manager->getWorld().entities.getAllDerivedComponents<Script>(true))
				script->update();
		}

		void lateUpdate()
		{
			for (Script::Handle &script : manager->getWorld().entities.getAllDerivedComponents<Script>(true))
				script->lateUpdate();
		}

		void fixedUpdate()
		{
			for (Script::Handle &script : manager->getWorld().entities.getAllDerivedComponents<Script>(true))
				script->fixedUpdate();
		}

	private:
	};
}

#endif