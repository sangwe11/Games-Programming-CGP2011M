#ifndef _CAMPFIRE_H
#define _CAMPFIRE_H

#include <EntitySystem/Entity.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/ParticleEmitter.h>
#include <Rendering/Light.h>

#include "../scripts/environment/FlickeringLight.h"

class Campfire : public EntitySystem::EntityTemplate
{
public:
	Campfire(const glm::vec3 &position) : position(position)
	{

	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		// Add transform
		entity.addComponent<Engine::Transform>(position, glm::vec3(0.0f), glm::vec3(1.0f));
		entity.addComponent<Engine::AudioSource>("sounds/Crackling_Fire.wav", 5, true, true);

		// Base
		EntitySystem::Entity base = entity.addChild();
		base.addComponent<Engine::Transform>(glm::vec3(0.0f, -0.2f, 0.0f), glm::vec3(0.0f), glm::vec3(0.3f));
		base.addComponent<Engine::MeshRenderer>("models/Environment/Campfire/fireplace.obj");

		// Fire fx
		EntitySystem::Entity fire = entity.addChild();
		fire.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		Engine::ParticleEmitter::Handle firefx = fire.addComponent<Engine::ParticleEmitter>(1000, 50.0f);
		firefx->addParticleStage(1500.0f, 0.5f, 0.5f, 4.0f, glm::vec4(1.0f, 0.27f, 0.0f, 1.0f), glm::vec4(1.0f, 0.1f, 0.0f, 0.1f), "textures/Particles/particle_fire1.png", 10, 4.0f, 1.0f, glm::vec3(-1.0f, 0.5f, -1.0f), glm::vec3(1.0f, 0.5f, 1.0f), 0.0f, 4.0f);

		// Smoke fx
		EntitySystem::Entity smoke = entity.addChild();
		smoke.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		Engine::ParticleEmitter::Handle smokefx = smoke.addComponent<Engine::ParticleEmitter>(1000, 50.0f);
		smokefx->addParticleStage(1500.0f, 0.5f, 0.5f, 3.0f, glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), "textures/default_particle.png", 1, 0.0f, 6.0f, glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
		smokefx->addParticleStage(2000.0f, -0.5f, -0.5f, 0.0f, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), "textures/default_particle.png", 0, 6.0f, 12.0f, glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f), 0.0f, 0.0f);

		// Light
		EntitySystem::Entity light = entity.addChild();
		light.addComponent<Engine::Transform>(glm::vec3(0.0f, 2.0f, 0.0), glm::vec3(0.0f), glm::vec3(1.0f));
		light.addComponent<Engine::PointLight>(glm::vec3(1.0f, 0.27f, 0.0f), 1.0f, 0.0f, 20.0f, 1.0f);
		light.addComponent<Engine::FlickeringLight>(0.6f, 1.0f, 25.0f, 32.0f, 0.05f);
	}

private:
	glm::vec3 position;
};

#endif