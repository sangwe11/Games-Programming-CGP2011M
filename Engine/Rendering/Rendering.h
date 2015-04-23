#ifndef _RENDERING_H
#define _RENDERING_H

#include <GLEW/glew.h>

#include "../EntitySystem/System.h"
#include "UniformBuffer.h"
#include "RenderingPrimative.h"
#include "Camera.h"
#include "Light.h"

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
		// Geometry passes
		void meshRender(Camera::Handle &camera);
		void billboardRender(Camera::Handle &camera);
		void particleRender(Camera::Handle &camera);
		void skyboxRender(Camera::Handle &camera);

		// Lighting passes
		void lighting(Camera::Handle &camera);
		void directionalPass(DirectionalLight::Handle &light, Shader &shader);
		void lightStencilPass(PointLight::Handle &light, Shader &shader);
		void pointLightPass(PointLight::Handle &light, Shader &shader);
		void lightStencilPass(SpotLight::Handle &light, Shader &shader);
		void spotLightPass(SpotLight::Handle &light, Shader &shader);

		// Post processing / final passes
		void postProcessing(Camera::Handle &camera, unsigned int &passes);
		void finalPass(Camera::Handle &camera, const unsigned int &postProcessingPasses);

		// Uniform buffers
		std::map<std::string, std::unique_ptr<UniformBuffer>> uniformBuffers;

		// Rendering primatives
		std::map<std::string, std::unique_ptr<RenderingPrimative>> renderingPrimatives;
	};
}

#endif