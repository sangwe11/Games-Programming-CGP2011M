#ifndef _ZOMBIEAI_H
#define _ZOMBIEAI_H

#include <Scripting/Script.h>
#include <Core/Transform.h>
#include <Audio/AudioSource.h>

#include "../PlayerController.h"

class ZombieAI : public Engine::Script<ZombieAI>
{
public:
	ZombieAI(const float &walkSpeed, const float &runSpeed);

	virtual void onAwake();
	virtual void update();

private:
	// Settings
	float walkSpeed;
	float runSpeed;

	// State
	bool tracking;
	float moanTimer = 3.0f;
	float hitTimer = 0.0f;

	// Components
	Engine::Transform::Handle transform;
	Engine::Transform::Handle player;
	Engine::PlayerController::Handle playerController;

	Engine::AudioSource::Handle moanSound;
};

#endif
