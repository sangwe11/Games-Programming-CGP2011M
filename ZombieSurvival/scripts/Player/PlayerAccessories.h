#ifndef _PLAYERTORCH_H
#define _PLAYERTORCH_H

#include <Scripting/Script.h>
#include <Rendering/Light.h>
#include <Rendering/UILabel.h>
#include <Rendering/Camera.h>

class PlayerAccessories : public Engine::Script<PlayerAccessories>
{
public:
	PlayerAccessories(const float &battery, const float &torchDrain, const float &nightVisionDrain);

	virtual void onAwake();
	virtual void update();

	void toggleTorch();
	void toggleNightVision();

private:
	// Settings
	float torchDrain;
	float nightVisionDrain;

	// State
	float batteryRemaining;
	bool torchOn;
	bool nightVisionOn;
	bool hasCharge;

	// Components
	Engine::SpotLight::Handle light;
	Engine::Camera::Handle camera;
	Engine::UILabel::Handle batteryUI;
};

#endif