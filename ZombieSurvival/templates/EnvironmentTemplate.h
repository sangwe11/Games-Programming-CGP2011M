#ifndef _ENVIRONMENTTEMPLATE_H
#define _ENVIRONMENTTEMPLATE_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/BillboardRenderer.h>
#include <Rendering/Light.h>
#include <Rendering/ParticleEmitter.h>
#include <Audio/AudioSource2D.h>
#include <Audio/AudioSource.h>

#include "../Scripts/Environment/FlickeringLight.h"

class EnvironmentTemplate : public EntitySystem::EntityTemplate
{
public:
	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		entity.tag("environment");
		entity.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		entity.addComponent<Engine::AudioSource2D>("sounds/hoard.mp3", 1, true, true);

		// Ambient lighting
		EntitySystem::Entity light = entity.addChild();
		light.addComponent<Engine::DirectionalLight>(glm::vec3(0.2f, 0.3f, 0.3), 0.0f, 0.025f, glm::vec3(0.0f, -1.0f, -1.0f));

		// Create the ground
		EntitySystem::Entity ground = entity.addChild();
		ground.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		ground.addComponent<Engine::MeshRenderer>("models/Environment/Ground/Ground.obj");

		// Create the campfire
		EntitySystem::Entity campfire = entity.addChild();
		campfire.addComponent<Engine::Transform>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		campfire.addComponent<Engine::AudioSource>("sounds/Crackling_Fire.wav", 5, true, true);

		// Base
		EntitySystem::Entity base = campfire.addChild();
		base.addComponent<Engine::Transform>(glm::vec3(0.0f, -0.2f, 0.0f), glm::vec3(0.0f), glm::vec3(0.3f));
		base.addComponent<Engine::MeshRenderer>("models/Environment/Campfire/fireplace.obj");

		// Fire fx
		EntitySystem::Entity fire = campfire.addChild();
		fire.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		Engine::ParticleEmitter::Handle firefx = fire.addComponent<Engine::ParticleEmitter>(1000, 50.0f);
		firefx->addParticleStage(1500.0f, 0.5f, 0.5f, 4.0f, glm::vec4(1.0f, 0.27f, 0.0f, 1.0f), glm::vec4(1.0f, 0.1f, 0.0f, 0.0f), "textures/Particles/particle_fire1.png", 10, 4.0f, 0.5f, glm::vec3(-1.0f, 0.5f, -1.0f), glm::vec3(1.0f, 0.5f, 1.0f), 0.0f, 8.0f);

		// Smoke fx
		EntitySystem::Entity smoke = campfire.addChild();
		smoke.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		Engine::ParticleEmitter::Handle smokefx = smoke.addComponent<Engine::ParticleEmitter>(1000, 50.0f);
		smokefx->addParticleStage(1500.0f, 0.5f, 0.5f, 3.0f, glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), "textures/default_particle.png", 1, 0.0f, 6.0f, glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
		smokefx->addParticleStage(2000.0f, -0.5f, -0.5f, 0.0f, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), "textures/default_particle.png", 0, 6.0f, 12.0f, glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f), 0.0f, 0.0f);

		// Light
		EntitySystem::Entity firelight = campfire.addChild();
		firelight.addComponent<Engine::Transform>(glm::vec3(0.0f, 2.0f, 0.0), glm::vec3(0.0f), glm::vec3(1.0f));
		firelight.addComponent<Engine::PointLight>(glm::vec3(1.0f, 0.27f, 0.0f), 1.0f, 0.0f, 20.0f, 1.0f);
		firelight.addComponent<Engine::FlickeringLight>(0.6f, 1.0f, 25.0f, 32.0f, 0.05f);

		// Create a billboard component to make a forest with
		EntitySystem::Entity forest = entity.addChild();
		forest.addComponent<Engine::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0));
		Engine::BillboardRenderer::Handle billboard = forest.addComponent<Engine::BillboardRenderer>("textures/Billboards/tree.png", glm::vec2(4.0f, 5.5f));
		
		// Generate forest
		for (unsigned int distance = 20; distance < 100; distance += 5)
		{
			for (unsigned int i = 0; i < 40; ++i)
			{
				float randomOffset = (rand() % 720) / 2.0f;
				float randomDistance = (rand() % 4) - 2.0f;

				glm::vec3 forward = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), Engine::DegToRads(36.0f * i + randomOffset), glm::vec3(0.0f, 1.0f, 0.0));

				glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f) + forward * (distance + randomDistance);

				billboard->addBillboard(position);
			}
		}
	}
};

#endif