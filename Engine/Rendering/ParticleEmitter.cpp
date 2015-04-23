#include "ParticleEmitter.h"
#include "../Core/Files.h"
#include "../Core/Transform.h"
#include "../EntitySystem/World.h"

namespace Engine
{
	ParticleEmitter::ParticleEmitter(const unsigned int &maxParticles, const float &launcherLifetime)
	{
		this->maxParticles = maxParticles;
		this->launcherLifetime = launcherLifetime;
		this->firstDraw = true;
		this->currentBuffer = 0;
		this->currentFeedback = 1;
	}

	void ParticleEmitter::initialise()
	{
		// Reserve space for max particles
		particles.reserve(maxParticles);

		// Get object transform
		Transform::Handle transform = entity.getComponent<Transform>();

		// Create launcher particle stage
		addParticleStage(launcherLifetime, 0.0f, 0.0f, false, glm::vec4(0.0f), glm::vec4(0.0f), "textures/default_particle.png", 1, 0.0f, 0.0f, glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f);

		// Add launcher particle
		particles.emplace_back(0, glm::vec3(0.0f), glm::vec3(0.0f), 0.0f);

		// Generate buffers and transform feedback
		glGenTransformFeedbacks(2, transformFeedback);
		glGenBuffers(2, particleBuffer);
		glGenVertexArrays(2, vaObject);

		// Bind and buffer data
		for (unsigned int i = 0; i < 2; ++i)
		{
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback[i]);
			glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(Particle)* maxParticles), &particles[0], GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[i]);

			glBindVertexArray(vaObject[i]);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);
		}
	}

	void ParticleEmitter::uninitialise()
	{
		glDeleteTransformFeedbacks(2, transformFeedback);
		glDeleteBuffers(2, particleBuffer);
		glDeleteVertexArrays(2, vaObject);
	}

	void ParticleEmitter::addParticleStage(const float &lifeTime, const float &startSpeed, const float &endSpeed, const float &gravityModifier, const glm::vec4 &startColor, const glm::vec4 &endColor, const std::string &texture, const unsigned int &subEmitCount, const float &startSize, const float &endSize, const glm::vec3 &minDirection, const glm::vec3 &maxDirection, const float &startRotation, const float &endRotation)
	{
		Texture2D& tex = manager->getWorld().systems.getSystem<Files>()->loadFile<Texture2D>(texture, true, false, false);

		// Add texture to list
		textures.push_back(tex);

		// Add to particle type list
		particleStages.emplace_back(lifeTime, startSpeed, endSpeed, gravityModifier, startColor, endColor, textures.size() - 1, subEmitCount, startSize, endSize, minDirection, maxDirection, startRotation, endRotation);
	}
}