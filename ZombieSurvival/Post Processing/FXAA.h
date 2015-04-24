#ifndef _FXAA_H
#define _FXAA_H

#include <Rendering/PostProcessingEffect.h>
#include <Rendering/Texture2D.h>

class FXAA : public Engine::PostProcessingEffect<FXAA>
{
public:
	FXAA();

	virtual void setUniforms();
};

#endif