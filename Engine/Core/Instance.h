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

		void setup(const std::string &title, const unsigned int &width, const unsigned int &height, const bool &fullscreen, const bool &vsync);
		void cleanup();
		void update();
	};
}

#endif