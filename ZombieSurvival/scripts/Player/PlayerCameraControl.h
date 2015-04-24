#ifndef _PLAYERCAMERACONTROL_H
#define _PLAYERCAMERACONTROL_H

#include <Scripting/Script.h>
#include <Core/Transform.h>
#include <Core/Input.h>
#include <Core/Time.h>

class PlayerCameraControl : public Engine::Script<PlayerCameraControl>
{
public:
	PlayerCameraControl(const float &sensitivity);

	virtual void onAwake();
	virtual void update();

	void toggleMouseLock();

private:
	// Settings
	float sensitivity = 2.0f;
	bool mouseLocked = false;

	// Systems
	Engine::Input::Handle input;
	Engine::Time::Handle time;

	// Components
	Engine::Transform::Handle transform;
	Engine::Transform::Handle parentTransform;
};

#endif