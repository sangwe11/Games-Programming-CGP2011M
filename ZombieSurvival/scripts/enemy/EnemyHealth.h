#ifndef _ENEMYHEALTH_H
#define _ENEMYHEALTH_H

#include <Scripting/Script.h>
#include "../Controllers/GameControl.h"

class EnemyHealth : public Engine::Script<EnemyHealth>
{
public:
	EnemyHealth(const float &startHealth, const float &maxHealth);

	virtual void onAwake();
	virtual void update();

	void addDamage(const float &amount);
	const bool alive() { return isAlive && (deathTimer <= 0.0f); }
	const float health() { return currentHealth; }

private:
	// Settings
	float maxHealth;

	// Systems
	Engine::Time::Handle time;

	// Components
	GameControl::Handle gameControl;

	// State
	float currentHealth;
	bool isAlive;
	float deathTimer;
};

#endif