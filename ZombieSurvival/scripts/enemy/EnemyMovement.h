#ifndef _ENEMYMOVEMENT_H
#define _ENEMYMOVEMENT_H

#include <Scripting/Script.h>
#include <Core/Transform.h>
#include <Audio/AudioSource.h>
#include <Core/Time.h>

#include "../Player/PlayerHealth.h"

class EnemyMovement : public Engine::Script<EnemyMovement>
{
public:
	EnemyMovement(const float &walkSpeed, const float &runSpeed);

	virtual void onAwake();
	virtual void update();

private:
	// Settings
	float walkSpeed;
	float runSpeed;

	// Systems
	Engine::Time::Handle time;

	// Components
	Engine::Transform::Handle transform;
	Engine::Transform::Handle player;
	Engine::AudioSource::Handle moanSounds[3];
	Engine::AudioSource::Handle hitSounds[3];
	PlayerHealth::Handle playerHealth;

	// State
	float hitTimer;
	float moanTimer;
};

#endif