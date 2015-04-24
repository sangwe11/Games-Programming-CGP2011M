#ifndef _WORLD_H
#define _WORLD_H

#include "Entity.h"
#include "System.h"

namespace EntitySystem
{
	class World
	{
	public:
		World() : entities(*this), systems(*this) { }

		EntityManager entities;
		SystemManager systems;
	};
}

#endif