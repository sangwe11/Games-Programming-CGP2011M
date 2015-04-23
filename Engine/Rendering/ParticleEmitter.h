#ifndef _PARTICLEEMITTER_H
#define _PARTICLEEMITTER_H

#include <GLM/glm.hpp>

#include "../EntitySystem/Entity.h"
#include "Texture2D.h" 

namespace Engine
{
	struct ParticleStage
	{
		ParticleStage(const float &lifeTime, const float &startSpeed, const float &endSpeed, const float &gravityModifier, const glm::vec4 &startColor, const glm::vec4 &endColor, const unsigned int &texture, const unsigned int &subEmitCount, const float &startSize, const float &endSize, const glm::vec3 &minDirection, const glm::vec3 &maxDirection, const float &startRotation, const float &endRotation)
		: lifeTime(lifeTime), startSpeed(startSpeed), endSpeed(endSpeed), gravityModifier(gravityModifier), startColor(startColor), endColor(endColor), texture(texture), subEmitCount(subEmitCount), startSize(startSize), endSize(endSize), minDirection(minDirection), maxDirection(maxDirection), startRotation(startRotation), endRotation(endRotation)
		{
		}

		float startSpeed;
		float endSpeed;
		float startSize;
		float endSize;

		float startRotation;
		float endRotation;
		float lifeTime;
		float gravityModifier;

		glm::vec4 startColor;

		glm::vec4 endColor;

		glm::vec3 minDirection;
		unsigned int texture;

		glm::vec3 maxDirection;
		unsigned int subEmitCount; // no of particles of this type to emit per particle if this is a sub emit
	};

	struct Particle
	{
		Particle(const int &type, const glm::vec3 &position, const glm::vec3 &velocity, const float &age)
		: type(type), position(position), velocity(velocity), age(age)
		{
		}

		int type;
		glm::vec3 position;
		glm::vec3 velocity;
		float age;
	};

	class ParticleEmitter : public EntitySystem::Component<ParticleEmitter>
	{
		friend class Rendering;
	public:
		ParticleEmitter(const unsigned int &maxParticles, const float &launcherLifetime);

		virtual void initialise();
		virtual void uninitialise();

		void addParticleStage(const float &lifeTime, const float &startSpeed, const float &endSpeed, const float &gravityModifier, const glm::vec4 &startColor, const glm::vec4 &endColor, const std::string &texture, const unsigned int &subEmitCount, const float &startSize, const float &endSize, const glm::vec3 &minDirection, const glm::vec3 &maxDirection, const float &startRotation, const float &endRotation);

		// Particle emitter settings
		unsigned int maxParticles;
		float launcherLifetime;

		// Particle lists
		std::vector<ParticleStage> particleStages;
		std::vector<Particle> particles;
		std::vector<Texture2D> textures;

		// Buffers and transform feedback
		GLuint particleBuffer[2];
		GLuint transformFeedback[2];

		// State
		GLuint vaObject[2];
		bool firstDraw;
		unsigned int lastTime;
		unsigned int currentBuffer;
		unsigned int currentFeedback;
	};
}

#endif