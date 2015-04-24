#ifndef _GAMECONTROL_H
#define _GAMECONTROL_H

#include <Scripting/Script.h>
#include <Rendering/UILabel.h>

#include "../Player/PlayerMovement.h"
#include "../Player/PlayerCameraControl.h"
#include "../Player/PlayerHealth.h"
#include "../Player/PlayerAccessories.h"
#include "../Enemy/EnemySpawning.h"

class GameControl : public Engine::Script<GameControl>
{
public:
	virtual void onAwake();
	virtual void update();

	void addScore(const unsigned int &amount);

private:
	// Systems
	Engine::Input::Handle input;

	// Components
	PlayerMovement::Handle movement;
	PlayerCameraControl::Handle camera;
	PlayerHealth::Handle health;
	PlayerAccessories::Handle accessories;
	EnemySpawning::Handle spawning;

	// UI components
	Engine::UILabel::Handle scoreUI;
	Engine::UILabel::Handle gamestartUI;
	Engine::UILabel::Handle gameoverUI;

	// State
	unsigned int score = 0;
	bool gameOver = false;
	bool started = false;
};

#endif