#include "PlayerCameraControl.h"

#include <Core/Controller.h>
#include <Core/KeyboardMouse.h>

PlayerCameraControl::PlayerCameraControl(const float &sensitivity) : sensitivity(sensitivity)
{
}

void PlayerCameraControl::onAwake()
{
	// Get transform component
	transform = entity.getComponent<Engine::Transform>();
	parentTransform = entity.getParent().getComponent<Engine::Transform>();

	// Get required systems
	input = entity.getManager().getWorld().systems.getSystem<Engine::Input>();
	time = entity.getManager().getWorld().systems.getSystem<Engine::Time>();
}

void PlayerCameraControl::update()
{
	// Input states
	glm::vec2 look;

	// Keyboard and controller
	if (input->exists<Engine::Controller>(0))
	{
		Engine::Controller &c = input->get<Engine::Controller>(0);

		// Use controller if present
		look.x = c.getAxis("lookX");
		look.y = c.getAxis("lookY");
	}
	else
	{
		Engine::KeyboardMouse &k = input->get<Engine::KeyboardMouse>(0);

		// Fallback to keyboard and mouse
		look.x = k.getAxis("lookX");
		look.y = k.getAxis("lookY");
	}

	// Rotate camera on vertical axis
	transform->rotateLocal(look.y * -sensitivity * time->getDeltaTime(), glm::vec3(1.0f, 0.0f, 0.0f));

	// Rotate parent on horizontal axis
	parentTransform->rotateGlobal(look.x * -sensitivity * time->getDeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PlayerCameraControl::toggleMouseLock()
{
	mouseLocked = !mouseLocked;
	SDL_SetRelativeMouseMode((SDL_bool)mouseLocked);
}