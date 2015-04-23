#ifndef _MEDICALPACK_H
#define _MEDICALPACK_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/Light.h>

#include "../scripts/pickups/HealthPack.h"

class MedicalPack : public EntitySystem::EntityTemplate
{
public:
	MedicalPack(const glm::vec3 &position) : position(position)
	{

	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		entity.addComponent<Engine::Transform>(glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		entity.addComponent<HealthPack>();

		EntitySystem::Entity mesh = entity.addChild();
		mesh.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(Engine::DegToRads(90.0f), 0.0f, 0.0f), glm::vec3(0.05f));
		mesh.addComponent<Engine::MeshRenderer>("models/Pickups/First_Aid_Med_Kit/FirstAidMedKit.obj");

		EntitySystem::Entity light = entity.addChild();
		light.addComponent<Engine::Transform>(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		light.addComponent<Engine::SpotLight>(glm::vec3(0.0f, 0.68f, 0.0f), 1.0f, 0.0f, 10.0f, glm::vec3(0.0f, -1.0f, 0.0f), 10.0f, 20.0f);
	}

private:
	glm::vec3 position;
};

#endif