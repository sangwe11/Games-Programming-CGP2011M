#ifndef _FOG_H
#define _FOG_H

#include <Rendering/PostProcessingEffect.h>
#include <Rendering/Texture2D.h>

class Fog : public Engine::PostProcessingEffect<Fog>
{
public:
	Fog();

	virtual void setUniforms();
};

#endif