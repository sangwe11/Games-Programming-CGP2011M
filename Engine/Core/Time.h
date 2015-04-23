#ifndef _TIME_H
#define _TIME_H

#include "../Engine/EntitySystem/System.h"

namespace Engine
{
	class Time : public EntitySystem::System<Time>
	{
	public:
		virtual void initialise();
		
		void update();
		void updateFixed();

		float getDeltaTime();

	private:
		float deltaTime = 0.0f;
		unsigned int lastTime = 0;
		bool first = true;

		// TODO: fixed timestepping for Physics
		//float fixedDeltaTime = 0.0f;

	};
}

#endif