#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>

#include "../scripts/enemy/ZombieAI.h"

class Zombie : public EntitySystem::EntityTemplate
{
public:
	Zombie(const glm::vec3 &position) : position(position)
	{

	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		entity.tag("zombie");
		entity.addComponent<Engine::Transform>(position, glm::vec3(0.0f), glm::vec3(1.0));
		entity.addComponent<ZombieAI>(1.5f, 3.0f);
		
		EntitySystem::Entity mesh = entity.addChild();
		mesh.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(Engine::DegToRads(-90.0f), 0.0f, 0.0f), glm::vec3(0.7f));
		mesh.addComponent<Engine::MeshRenderer>("models/Enemy/ZombiePoliceman/Zombie_Policeman_A.dae");
	}

private:
	glm::vec3 position;
};

#endif