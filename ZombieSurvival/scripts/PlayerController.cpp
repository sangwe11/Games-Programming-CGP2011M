#include "PlayerController.h"

#include <Core/Maths.h>
#include <Core/Time.h>
#include <Core/Input.h>
#include <Core/Controller.h>
#include <Core/KeyboardMouse.h>
#include <EntitySystem/World.h>
#include <Rendering/MeshRenderer.h>
#include <Rendering/Light.h>
#include <Audio/AudioListener.h>

namespace Engine
{
	PlayerController::PlayerController(const float &walkSpeed, const float &runSpeed, const bool &canJump, const float &jumpHeight)
	{
		this->walkSpeed = walkSpeed;
		this->runSpeed = runSpeed;
		this->canJump = canJump;
		this->jumpHeight = jumpHeight;
		this->isGrounded = false;
		this->nightVision = false;
	}

	void PlayerController::onAwake()
	{
		// Get transform and camera components
		transform = entity.getComponent<Transform>();
		camera = entity.getComponent<Camera>();

		// Add audio listener
		entity.addComponent<Engine::AudioListener>();

		// Add audio sources
		footstepSound = entity.addComponent<Engine::AudioSource>("sounds/footsteps.mp3", 10, true, false);
		//jumpSound = entity.addComponent<AudioSource>("Player/JumpSound.wav", 20);
		//nightVisionSound = entity.addComponent<AudioSource>("Player/NightVisionWarmUp.mp3", 20);

		// Add spotlight
		spotlight = entity.addComponent<Engine::SpotLight>(glm::vec3(1.0f), 1.0f, 0.0f, 40.0f, glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, 20.0f);

		// Add player health
		health = entity.addComponent<PlayerHealth>();		
	}

	void PlayerController::update()
	{
		// Input manager handle
		Input::Handle input = manager->getWorld().systems.getSystem<Input>();

		// Deltatime
		float deltaTime = manager->getWorld().systems.getSystem<Time>()->getDeltaTime();

		// Input states
		glm::vec2 move;
		glm::vec2 look;
		bool sprint;
		bool jump;
		bool toggleLight;

		// Keyboard and controller
		if (input->exists<Engine::Controller>(0))
		{
			Engine::Controller &c = input->get<Engine::Controller>(0);

			// Use controller if present
			move.x = c.getAxis("moveX");
			move.y = c.getAxis("moveY");
			look.x = c.getAxis("lookX");
			look.y = c.getAxis("lookY");
			sprint = c.getButton("sprint");
			jump = c.getButtonDown("jump");
			toggleLight = c.getButtonDown("special1");
		}
		else
		{
			Engine::KeyboardMouse &k = input->get<Engine::KeyboardMouse>(0);

			// Fallback to keyboard and mouse
			move.x = k.getAxis("moveX");
			move.y = k.getAxis("moveY");
			look.x = k.getAxis("lookX");
			look.y = k.getAxis("lookY");
			sprint = k.getButton("sprint");
			jump = k.getButtonDown("jump");
			toggleLight = k.getButtonDown("special1");
		}

		// Some basic player movement
		glm::vec3 forward = transform->forward();
		forward.y = 0.0f;
		forward = glm::normalize(forward);

		// Calculate direction
		glm::vec3 direction;
		direction += forward * -move.y;
		direction += transform->right() * move.x;

		// Is there a direction?
		if (direction.x != 0.0f || direction.z != 0.0f)
		{
			// Do we need to normalise?
			if (glm::length(direction) > 1.0f)
				direction = glm::normalize(direction);

			footstepSound->play();

			// Add movement
			if (sprint)
				transform->position += direction * 6.0f * deltaTime;
			else
				transform->position += direction * 4.0f * deltaTime;
		}
		else
		{
			footstepSound->stop();
		}

		// Rotate camera on look axis
		transform->rotateGlobal(look.x * -2.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		transform->rotateLocal(look.y * -2.0f * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));

		// Toggle light
		if (toggleLight)
		{
			if (spotlight->isEnabled())
				spotlight->disable();
			else
				spotlight->enable();
		}

		if (jump)
		{
			health->addDamage(10.0f);
		}

		//// Jumping
		//if (keystate[SDL_SCANCODE_SPACE] && isGrounded)
		//{
		//	rigidbody->velocity = glm::vec3(rigidbody->velocity.x, CalculateJumpSpeed(), rigidbody->velocity.z);
		//	isGrounded = false;
		//	jumpSound->Play();
		//}
	}
}