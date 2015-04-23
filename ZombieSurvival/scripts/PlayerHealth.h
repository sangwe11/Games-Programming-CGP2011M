#ifndef _PLAYERHEALTH_H
#define _PLAYERHEALTH_H

#include <Scripting/Script.h>
#include <Rendering/UILabel.h>

class PlayerHealth : public Engine::Script<PlayerHealth>
{
public:
	PlayerHealth(const float &startHealth = 100.0f, const float &maxHealth = 100.0f, const bool &regenerate = true, const float &regenerateRate = 0.5f);

	virtual void onAwake();
	virtual void update();

	const bool getAlive() const { return alive; }
	void addDamage(const float &damage) { currentHealth -= damage; }
	void addHealth(const float &amount) { currentHealth += amount; }

private:
	// Settings
	float maxHealth;
	bool regenerate;
	float regenerateRate;
	bool alive;

	// State
	float currentHealth;

	// Components
	Engine::UILabel::Handle healthLabel;
};

#endif
