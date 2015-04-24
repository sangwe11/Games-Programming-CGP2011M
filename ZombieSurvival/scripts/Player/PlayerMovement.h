#ifndef _PLAYERMOVEMENT_H
#define _PLAYERMOVEMENT_H

#include <Scripting/Script.h>
#include <Core/Transform.h>
#include <Core/Input.h>
#include <Core/Time.h>

class PlayerMovement : public Engine::Script<PlayerMovement>
{
public:
	PlayerMovement(const float &walkSpeed, const float &runSpeed, const float &jumpHeight);

	virtual void onAwake();
	virtual void update();

private:
	// Settings
	float walkSpeed;
	float runSpeed;
	float jumpHeight;

	// Systems
	Engine::Input::Handle input;
	Engine::Time::Handle time;

	// Components
	Engine::Transform::Handle transform;
	Engine::Transform::Handle axe;

	// State
	float attackTimer = 0.0f;
	bool mouseCaptured = false;
};

#endif