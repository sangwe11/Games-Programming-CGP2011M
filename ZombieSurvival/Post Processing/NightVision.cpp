#include "NightVision.h"

NightVision::NightVision(float colorAmplication, glm::vec3 visionColor) : PostProcessingEffect("shaders/Post Processing/nightvision")
{
	this->colorAmplication = colorAmplication;
	this->visionColor = visionColor;
}

void NightVision::setUniforms()
{
	// Use textures
	files->loadFile<Engine::Texture2D>("textures/PostProcessing/NightVisionOverlay.png", true, false, 0).use(1);
	files->loadFile<Engine::Texture2D>("textures/PostProcessing/NightVisionNoise.png", true, false, 0).use(2);

	// Set uniforms
	shader.setUniform("overlayTexture", 1);
	shader.setUniform("noiseTexture", 2);
	shader.setUniform("colorAmplification", colorAmplication);
	shader.setUniform("visionColor", visionColor);
	shader.setUniform("elapsedTime", (SDL_GetTicks() - startTime) / 1000.0f);
}