#ifndef _PLAYERHEALTH_H
#define _PLAYERHEALTH_H

#include <string>

#include <Scripting/Script.h>
#include <Rendering/UILabel.h>
#include <Audio/AudioSource.h>

class PlayerHealth : public Engine::Script<PlayerHealth>
{
public:
	PlayerHealth(const float &startHealth, const float &maxHealth, const std::string &healthPrefix = "Health:");

	virtual void onAwake();
	virtual void update();
	virtual void onDestroy();

	const float health() { return currentHealth; }
	const bool alive() { return isAlive; }

	void addDamage(const float &amount);
	void addHealth(const float &amount);

private:
	// Settings
	float maxHealth;
	std::string healthPrefix;

	// Components
	Engine::UILabel::Handle healthUI;
	Engine::AudioSource::Handle hitSound;

	// State
	float currentHealth;
	bool isAlive;
};

#endif