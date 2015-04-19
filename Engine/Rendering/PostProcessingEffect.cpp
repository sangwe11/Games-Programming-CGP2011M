#include "PostProcessingEffect.h"
#include "../Core/Files.h"

namespace Engine
{
	BasePostProcessingEffect::BasePostProcessingEffect(const std::string &shaderName)
	{
		// Store shader file
		this->shaderName = shaderName;

		// Disabled by default
		enabled = false;
	}

	Shader &BasePostProcessingEffect::getShader()
	{
		return shader;
	}

	void BasePostProcessingEffect::enable()
	{
		enabled = true;
	}

	void BasePostProcessingEffect::disable()
	{
		enabled = false;
	}

	bool BasePostProcessingEffect::isEnabled()
	{
		return enabled;
	}

	void BasePostProcessingEffect::loadShader()
	{
		shader = files->loadFile<Shader>(shaderName);
	}
}