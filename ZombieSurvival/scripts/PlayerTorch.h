#ifndef _PLAYERTORCH_H
#define _PLAYERTORCH_H

#include <Scripting/Script.h>
#include <Rendering/Light.h>
#include <Rendering/UILabel.h>

class PlayerTorch : public Engine::Script<PlayerTorch>
{
public:
	PlayerTorch(const float &drainRate);

	virtual void onAwake();
	virtual void update();

	void toggle();

private:
	// Settings
	float drainRate;

	// State
	float batteryRemaining;
	bool on;
	bool hasCharge;

	// Components
	Engine::SpotLight::Handle light;
	Engine::UILabel::Handle batteryLabel;
};

#endif