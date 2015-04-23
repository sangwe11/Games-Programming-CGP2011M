#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H

#include <Scripting/Script.h>
#include <Core/Transform.h>
#include <Rendering/Camera.h>
#include <Audio/AudioSource.h>
#include <Rendering/Light.h>

#include "PlayerHealth.h"
#include "PlayerTorch.h"

namespace Engine
{
	class PlayerController : public Script<PlayerController>
	{
	public:
		PlayerController(const float &walkSpeed = 6.0f, const float &runSpeed = 9.0f, const bool &canJump = true, const float &jumpHeight = 2.0f);

		virtual void onAwake();
		virtual void update();

		PlayerHealth::Handle &getHealth() { return health; }

	private:
		// Settings
		float walkSpeed;
		float runSpeed;
		float jumpHeight;
		bool canJump;

		// Components
		Transform::Handle transform;
		Camera::Handle camera;
		Transform::Handle cameraTransform;
		PlayerHealth::Handle health;
		PlayerTorch::Handle torch;

		// Sound effects
		AudioSource::Handle jumpSound;
		AudioSource::Handle nightVisionSound;
		AudioSource::Handle footstepSound;

		// State
		bool isGrounded;
		bool nightVision;
		unsigned int lastSpawnTime = 0;
		unsigned int lastSpawnTimeSphere = 0;
	};
}
#endif
