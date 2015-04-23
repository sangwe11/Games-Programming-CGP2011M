#ifndef _ZOMBIESPAWNING_H
#define _ZOMBIESPAWNING_H

#include <Scripting/Script.h>
#include <Audio/AudioSource2D.h>
#include <Rendering/UILabel.h>

class ZombieSpawning : public Engine::Script<ZombieSpawning>
{
public:
	ZombieSpawning(const float &roundTime, const unsigned int &startCount, const float &roundMultiplier);

	virtual void onAwake();
	virtual void update();

private:
	void nextRound();

	// Settings
	float roundTime;
	unsigned int startCount;
	float roundMultiplier;

	// State
	float spawnDelay;
	unsigned int currentRound;
	unsigned int leftToSpawn;
	float spawnTimer;
	float newRoundTimer;
	bool messageShown;
	bool roundSpawned;
	bool roundFinished;

	// Components
	Engine::AudioSource2D::Handle nextRoundSound;
	Engine::UILabel::Handle nextRoundText;
};

#endif