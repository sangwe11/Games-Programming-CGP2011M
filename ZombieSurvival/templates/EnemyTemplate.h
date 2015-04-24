#ifndef _ENEMYTEMPLATE_H
#define _ENEMYTEMPLATE_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>

#include "../Scripts/Enemy/EnemyMovement.h"
#include "../Scripts/Enemy/EnemyHealth.h"

class EnemyTemplate : public EntitySystem::EntityTemplate
{
public:
	EnemyTemplate(const glm::vec3 &position)
	{
		startPosition = position;
	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		// Add a slight random bias to speed
		float speedBias = ((rand() % 1000) / 1000.0f) - 0.5f;

		entity.tag("zombie");
		entity.addComponent<Engine::Transform>(startPosition, glm::vec3(0.0f), glm::vec3(1.0f));
		entity.addComponent<EnemyMovement>(1.5f + speedBias, 2.5f + speedBias);
		entity.addComponent<EnemyHealth>(10.0f, 10.0f);

		EntitySystem::Entity mesh = entity.addChild();
		mesh.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(Engine::DegToRads(-90.0f), 0.0f, 0.0f), glm::vec3(0.7f));
		mesh.addComponent<Engine::MeshRenderer>("models/Enemy/ZombiePoliceman/Zombie_Policeman_A.dae");
	}

private:
	glm::vec3 startPosition;
};

#endif