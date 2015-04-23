#ifndef _PLAYER_H
#define _PLAYER_H

#include <EntitySystem/Entity.h>
#include <Core/Maths.h>
#include <Core/Transform.h>
#include <Rendering/Skybox.h>

#include "../scripts/PlayerController.h"
#include "../scripts/PlayerTorch.h"

#include "../postprocessing/FXAA.h"
#include "../postprocessing/Fog.h"

class Player : public EntitySystem::EntityTemplate
{
public:
	Player(const glm::vec3 &position) : position(position)
	{

	}

	virtual void build(EntitySystem::Entity &entity, EntitySystem::EntityManager &entities)
	{
		// Tag as player
		entity.tag("player");

		// Add transform
		entity.addComponent<Engine::Transform>(position, glm::vec3(0.0f), glm::vec3(1.0));

		// Add camera child
		EntitySystem::Entity camera = entity.addChild();
		camera.addComponent<Engine::Transform>(glm::vec3(0.0f, 2.4f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		Engine::Camera::Handle cam = camera.addComponent<Engine::Camera>(60.0f, 0.3f, 1000.0f, true, false, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		camera.addComponent<Engine::Skybox>("textures/Skyboxes/ame_starfield", "starfield_rt.tga", "starfield_lf.tga", "starfield_up.tga", "starfield_dn.tga", "starfield_bk.tga", "starfield_ft.tga");
	
		// Add camera post processing (fxaa and fog)
		cam->addPostProcessingEffect<FXAA>();
		//cam->addPostProcessingEffect<Fog>().enable();

		// Add torch child
		EntitySystem::Entity torch = camera.addChild();
		torch.addComponent<Engine::Transform>(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		torch.addComponent<PlayerTorch>(2.0f);

		// Add player controller
		entity.addComponent<Engine::PlayerController>(4.0f, 6.0f, true, 2.0f);
	
	}

private:
	glm::vec3 position;
};

#endif