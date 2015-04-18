#ifndef _INSTANCE_H
#define _INSTANCE_H

#include "../EntitySystem/World.h"

namespace Engine
{
	class Instance : public EntitySystem::World
	{
	public:
		Instance();
		~Instance();

		void setup();
		void cleanup();
		void update();
	};
}

#endif