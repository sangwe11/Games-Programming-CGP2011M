#include "FXAA.h"

#include <Rendering/Display.h>

FXAA::FXAA() : PostProcessingEffect("shaders/Post Processing/fxaa")
{
}

void FXAA::setUniforms()
{
	const unsigned int width = files->getManager().getSystem<Engine::Display>()->getWidth();
	const unsigned int height = files->getManager().getSystem<Engine::Display>()->getHeight();

	// Set uniforms
	shader.setUniform("inverseTextureSize", glm::vec2(1.0f / (float)width, 1.0f / (float)height));
	shader.setUniform("fxaaSpanMax", 8.0f);
	shader.setUniform("fxaaReduceMin", 1.0f / 128.0f);
	shader.setUniform("fxaaReduceMul", 1.0f / 8.0f);
}