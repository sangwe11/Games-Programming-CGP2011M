#ifndef _PLAYERTEMPLATE_H
#define _PLAYERTEMPLATE_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/Camera.h>
#include <Rendering/Skybox.h>
#include <Rendering/MeshRenderer.h>
#include <Audio/AudioListener.h>

#include "../Scripts/Player/PlayerMovement.h"
#include "../Scripts/Player/PlayerCameraControl.h"
#include "../Scripts/Player/PlayerHealth.h"
#include "../Scripts/Player/PlayerAccessories.h"

#include "../Post Processing/FXAA.h"
#include "../Post Processing/NightVision.h"


class PlayerTemplate : public EntitySystem::EntityTemplate
{
public:
	PlayerTemplate(const glm::vec3 &position)
	{
		spawnPosition = position;
	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		// Tag and add components
		entity.tag("player");
		entity.addComponent<Engine::Transform>(spawnPosition, glm::vec3(0.0f), glm::vec3(1.0f));
		entity.addComponent<Engine::AudioListener>();
		entity.addComponent<PlayerHealth>(100.0f, 100.0f, "Health:");

		// Add camera
		EntitySystem::Entity camera = entity.addChild();
		camera.addComponent<Engine::Transform>(glm::vec3(0.0f, 2.4f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		camera.addComponent<Engine::Camera>(60.0f, 0.3f, 1000.0f, true, false, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		camera.getComponent<Engine::Camera>()->addPostProcessingEffect<FXAA>().enable();
		camera.addComponent<Engine::Skybox>("textures/Skyboxes/ame_starfield", "starfield_rt.tga", "starfield_lf.tga", "starfield_up.tga", "starfield_dn.tga", "starfield_bk.tga", "starfield_ft.tga");
		camera.addComponent<PlayerCameraControl>(2.0f);
		camera.addComponent<PlayerAccessories>(200.0f, 1.0f, 4.0f);

		// Add weapon
		EntitySystem::Entity axe = camera.addChild();
		axe.tag("axe");
		axe.addComponent<Engine::Transform>(glm::vec3(0.5f, -1.1f, 0.0f), glm::vec3(Engine::DegToRads(-30.0f), Engine::DegToRads(10.0f), 0.0f), glm::vec3(1.0f));
		EntitySystem::Entity mesh = axe.addChild();
		mesh.addComponent<Engine::Transform>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, Engine::DegToRads(180.0f), Engine::DegToRads(90.0f)), glm::vec3(0.1f));
		mesh.addComponent<Engine::MeshRenderer>("models/Weapons/Axe/Axe.obj");

		entity.addComponent<PlayerMovement>(4.0f, 6.0f, 4.0f);
	}

private:
	glm::vec3 spawnPosition;
};

#endif