#ifndef _SPAWNPOINTS_H
#define _SPAWNPOINTS_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>

#include "../scripts/enemy/ZombieAI.h"

class SpawnPoints : public EntitySystem::EntityTemplate
{
public:
	SpawnPoints(const glm::vec3 &position) : position(position)
	{

	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		entity.addComponent<Engine::Transform>(position, glm::vec3(0.0f), glm::vec3(1.0));
		entity.tag("spawn_points");

		unsigned int c = 0;

		for (unsigned int distance = 80; distance < 100; distance += 20)
		{
			for (unsigned int i = 0; i < 1; ++i)
			{
				glm::vec3 forward = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), Engine::DegToRads((36.0f * i) + (distance * i) / 72.0f), glm::vec3(0.0f, 1.0f, 0.0));

				EntitySystem::Entity spawnpoint = entity.addChild();
				spawnpoint.addComponent<Engine::Transform>(forward * (float)distance, glm::vec3(0.0f), glm::vec3(0.5f));
				//spawnpoint.addComponent<Engine::MeshRenderer>("models/Environment/Crate/Crate.obj");

				++c;
			}
		}

		std::cout << "Spawn points: " << c << std::endl;
	}

private:
	glm::vec3 position;
};

#endif