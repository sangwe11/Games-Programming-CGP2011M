#ifndef _SPAWNPOINTSTEMPLATE_H
#define _SPAWNPOINTSTEMPLATE_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>

class SpawnPointsTemplate : public EntitySystem::EntityTemplate
{
public:
	SpawnPointsTemplate(const glm::vec3 &center) : centrePosition(center)
	{

	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		unsigned int count = 36;
		float angleBetween = 360.0f / count;
		float minDistance = 30.0f;
		float maxDistance = 50.0f;

		// Creates a ring of evenly placed spawn points around the parent entity, with random distances from the center point
		for (unsigned int i = 0; i < count; ++i)
		{
			glm::vec3 forward = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), Engine::DegToRads(angleBetween * i), glm::vec3(0.0f, 1.0f, 0.0f));
			float distance = glm::mix(minDistance, maxDistance, (rand() % 100) / 100.0f);

			EntitySystem::Entity spawnPoint = entity.addChild();
			spawnPoint.tag("spawnpoint");
			spawnPoint.addComponent<Engine::Transform>(centrePosition + (forward * distance), glm::vec3(0.0f), glm::vec3(1.0f));
		}
	}

private:
	glm::vec3 centrePosition;
};

#endif