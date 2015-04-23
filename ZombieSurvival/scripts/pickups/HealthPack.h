#ifndef _HEALTHPACK_H
#define _HEALTHPACK_H

#include <Scripting/Script.h>
#include <Core/Transform.h>
#include <Audio/AudioSource.h>
#include "../PlayerHealth.h"

class HealthPack : public Engine::Script<HealthPack>
{
public:
	virtual void onAwake();
	virtual void update();

private:
	Engine::Transform::Handle transform;
	Engine::Transform::Handle player;
	PlayerHealth::Handle playerHealth;
	Engine::AudioSource::Handle sound;
};

#endif