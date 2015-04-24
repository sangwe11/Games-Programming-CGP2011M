#ifndef _ENEMYSPAWNING_H
#define _ENEMYSPAWNING_H

#include <Scripting/Script.h>
#include <Core/Time.h>
#include <Audio/AudioSource2D.h>
#include <Rendering/UILabel.h>

class EnemySpawning : public Engine::Script<EnemySpawning>
{
public:
	EnemySpawning(const unsigned int &firstRoundCount, const unsigned int &firstRoundMax, const float &difficulty);

	virtual void onAwake();
	virtual void update();

	void nextRound();

private:

	void spawn();

	// Settings
	unsigned int firstRoundCount;
	unsigned int firstRoundMax;
	float difficulty;

	// Systems
	Engine::Time::Handle time;

	// Components
	Engine::AudioSource2D::Handle roundSound;
	Engine::UILabel::Handle currentRoundUI;

	// State
	unsigned int currentRound = 0;
	float roundTime = 0.0f;
	unsigned int max;
	unsigned int left;
};

#endif