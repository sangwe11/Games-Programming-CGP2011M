#ifndef _RENDERING_H
#define _RENDERING_H

#include <GLEW/glew.h>

#include "../EntitySystem/System.h"
#include "UniformBuffer.h"
#include "RenderingPrimative.h"
#include "Camera.h"

namespace Engine
{
	class Rendering : public EntitySystem::System<Rendering>
	{
	public:
		virtual void initialise();
		virtual void uninitialise();

		void render();

		UniformBuffer &newUniformBuffer(const std::string &name, unsigned int size);
		bool uniformBufferExists(const std::string &name);
		UniformBuffer &getUniformBuffer(const std::string &name);

	private:
		void meshRender(Camera::Handle &camera);
		void skyboxRender(Camera::Handle &camera);
		void finalPass(Camera::Handle &camera);

		// Uniform buffers
		std::map<std::string, std::unique_ptr<UniformBuffer>> uniformBuffers;

		// Rendering primatives
		std::map<std::string, std::unique_ptr<RenderingPrimative>> renderingPrimatives;
	};
}

#endif