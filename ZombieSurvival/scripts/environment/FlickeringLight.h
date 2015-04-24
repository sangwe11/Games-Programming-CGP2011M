#ifndef _FLICKERINGLIGHT_H
#define _FLICKERINGLIGHT_H

#include <Scripting/Script.h>
#include <Rendering/Light.h>

namespace Engine
{
	class FlickeringLight : public Engine::Script<FlickeringLight>
	{
	public:
		FlickeringLight(const float &min = 0.0f, const float &max = 1.0f, const float &minRange = 10.0f, const float &maxRange = 20.0f, const float &delay = 0.01f);

		virtual void onAwake();
		virtual void update();

	private:
		// Settings
		float minIntensity;
		float maxIntensity;
		float minRange;
		float maxRange;
		float delay;

		// State
		float timer = 0.0f;

		// Components
		Engine::PointLight::Handle light;
	};
}

#endif
