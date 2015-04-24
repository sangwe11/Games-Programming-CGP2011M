#ifndef _NIGHTVISION_H
#define _NIGHTVISION_H

#include <Rendering/PostProcessingEffect.h>
#include <Rendering/Texture2D.h>

class NightVision : public Engine::PostProcessingEffect<NightVision>
{
public:
	NightVision(float colorAmplication = 8.0f, glm::vec3 visionColor = glm::vec3(0.1, 0.95, 0.2));

	virtual void setUniforms();

private:
	float colorAmplication;
	glm::vec3 visionColor;
	unsigned int startTime;
};

#endif