#include "Fog.h"

#include <Rendering/Display.h>

Fog::Fog() : PostProcessingEffect("shaders/Post Processing/fog")
{
	// Require the depth texture to be bound
	depthTexture = true;
}

void Fog::setUniforms()
{
	const unsigned int width = files->getManager().getSystem<Engine::Display>()->getWidth();
	const unsigned int height = files->getManager().getSystem<Engine::Display>()->getHeight();

	shader.setUniform("screensize", glm::vec2(width, height));
}