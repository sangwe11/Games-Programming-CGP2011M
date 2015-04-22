#include "Rendering.h"
#include <GLEW/glew.h>
#include <string>
#include <GLM/ext.hpp>

#include "../EntitySystem/World.h"
#include "Display.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "../Core/Transform.h"
#include "Skybox.h"

namespace Engine
{
	void Rendering::initialise()
	{
		// Add function to main
		addUpdateFunction(&Rendering::render, *this, 25);

		// Create uniform buffers
		newUniformBuffer("cameraUniforms", sizeof(glm::mat4) * 2);
		newUniformBuffer("transformUniforms", sizeof(glm::mat4));

		// Create rendering primatives
		renderingPrimatives["framebufferQuad"] = std::make_unique<RenderingQuad>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Post Processing/final_pass"));
		renderingPrimatives["lightingQuad"] = std::make_unique<RenderingQuad>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Lighting/lighting_directional"));
		renderingPrimatives["skyboxCube"] = std::make_unique<RenderingCube>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Geometry/skybox"));
		renderingPrimatives["lightingSphere"] = std::make_unique<RenderingSphere>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Lighting/lighting_point"), 2);
	}

	void Rendering::uninitialise()
	{
		// Clear rendering primatives list
		renderingPrimatives.clear();

		// Clear uniform buffer list
		uniformBuffers.clear();
	}

	void Rendering::render()
	{
		// Get camera uniform buffer
		UniformBuffer &cameraBuffer = getUniformBuffer("cameraUniforms");

		for (Camera::Handle &camera : manager->getWorld().entities.getAllComponents<Camera>(true))
		{
			unsigned int postProcessingPasses = 0;

			// Get the cameras framebuffer
			Framebuffer &framebuffer = camera->getFramebuffer();

			// Buffer uniform data
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer.getBuffer());
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera->getView()[0][0]);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camera->getProjection()[0][0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// Clear textures and depth ready for drawing (Diffuse, Position, Normal and Specular)
			framebuffer.clear({ 0, 1, 2, 3 }, true, false);

			// Render geometry
			meshRender(camera);

			// Render skybox
			if (camera.getEntity().hasComponent<Skybox>())
				skyboxRender(camera);

			framebuffer.clear({ 4 }, false, false);

			// Lighting
			if (camera->getLighting())
				lighting(camera);

			// Post processing
			postProcessing(camera, postProcessingPasses);

			// Render camera to screen
			finalPass(camera, postProcessingPasses);

			// Unbind framebuffer
			framebuffer.unbind(true, true);
		}
	}

	void Rendering::meshRender(Camera::Handle &camera)
	{
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// Get handle to file system
		Files::Handle files = manager->getSystem<Files>();

		// Load and use shader
		Shader &shader = files->loadFile<Shader>("shaders/Geometry/mesh_render");
		shader.use();

		// Get transform uniform buffer
		UniformBuffer &transformBuffer = getUniformBuffer("transformUniforms");

		// Render
		for (MeshRenderer::Handle &meshrenderer : manager->getWorld().entities.getAllComponents<MeshRenderer>(true))
		{
			// Get transform component
			Transform::Handle &transform = meshrenderer->entity.getComponent<Transform>();

			// Get model matrix
			glm::mat4 t = transform->getModelMatrix();

			// Buffer uniform data
			glBindBuffer(GL_UNIFORM_BUFFER, transformBuffer.getBuffer());
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &t[0][0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// Load model
			Model &model = files->loadFile<Model>(meshrenderer->file);

			// Draw
			model.draw(shader);
		}
	}

	void Rendering::skyboxRender(Camera::Handle &camera)
	{
		// Get skybox component attached to camera
		Skybox::Handle skybox = camera.getEntity().getComponent<Skybox>();

		// Get handle to file system
		Files::Handle files = manager->getSystem<Files>();

		// Load and use shader
		Shader &shader = files->loadFile<Shader>("shaders/Geometry/skybox");
		shader.use();

		// If using lighting, draw to final texture, otherwise draw to diffuse texture
		if (camera->getLighting())
			camera->getFramebuffer().bindDrawbuffers({ 4 });
		else
			camera->getFramebuffer().bindDrawbuffers({ 1 });

		// Use skybox texture
		skybox->texture.use(0);

		// Set uniforms
		shader.setUniform("cubemapTexture", 0);

		// Get transform uniform buffer
		UniformBuffer &transformBuffer = getUniformBuffer("transformUniforms");

		// Get transform component
		Transform::Handle &transform = skybox->entity.getComponent<Transform>();

		// Get model matrix
		glm::mat4 t = transform->getModelMatrix();

		// Buffer uniform data
		glBindBuffer(GL_UNIFORM_BUFFER, transformBuffer.getBuffer());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &t[0][0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Cull front faces
		glCullFace(GL_FRONT);

		// Load cube primative model
		Model &cube = files->loadFile<Model>("models/Primatives/Cube.obj");

		// Draw skybox cube
		cube.draw(shader);

		// TODO: look into why the cube rendering primative fails.
		//glBindVertexArray(renderingPrimatives["skyboxCube"]->vaObject);
		//glDrawElements(GL_TRIANGLES, renderingPrimatives["skyboxCube"]->drawCount, GL_UNSIGNED_INT, 0);

		// Restore backface culling
		glCullFace(GL_BACK);
	}

	void Rendering::lighting(Camera::Handle &camera)
	{
		// Bind textures for reading (Position, Diffuse + opacity, Normal, Specular color + exponent)
		camera->getFramebuffer().bindTextures({ 0, 1, 2, 3 });

		// Bind final texture as drawbuffer
		camera->getFramebuffer().bindDrawbuffers({ 4 });

		// Enable blending
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		// Disable depth test
		glDisable(GL_DEPTH_TEST);

		// Get handle to file system
		Files::Handle files = manager->getSystem<Files>();

		// Load shaders
		Shader &directionalShader = files->loadFile<Shader>("shaders/Lighting/lighting_directional");
		Shader &stencilShader = files->loadFile<Shader>("shaders/Lighting/lighting_stencil");
		Shader &pointShader = files->loadFile<Shader>("shaders/Lighting/lighting_point");
		Shader &spotShader = files->loadFile<Shader>("shaders/Lighting/lighting_spot");
		
		// Use directional light shader
		directionalShader.use();

		// Set uniforms
		directionalShader.setUniform("diffuseOpacityIn", 1);
		directionalShader.setUniform("normalIn", 2);
		directionalShader.setUniform("screenSize", manager->getSystem<Display>()->getSize());

		// Render directional lights
		for (DirectionalLight::Handle &light : manager->getWorld().entities.getAllComponents<DirectionalLight>(true))
			directionalPass(light, directionalShader);

		// Enable stencil test for lighting volumes
		glEnable(GL_STENCIL_TEST);

		// Get transform uniform buffer
		UniformBuffer &transformBuffer = getUniformBuffer("transformUniforms");

		// Render point lights
		for (PointLight::Handle &light : manager->getWorld().entities.getAllComponents<PointLight>(true))
		{
			// Create model matrix to scale sphere
			Transform::Handle transform = light.getEntity().getComponent<Transform>();
			glm::mat4 lightMatrix = glm::translate(transform->position) * glm::mat4_cast(transform->getRotation()) * glm::scale(glm::vec3(light->radius));

			// Buffer uniform data
			glBindBuffer(GL_UNIFORM_BUFFER, transformBuffer.getBuffer());
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &lightMatrix[0][0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// Clear stencil buffer only
			camera->getFramebuffer().clear({}, false, true);

			// Setup for stencil pass
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glDisable(GL_CULL_FACE);
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			// Stencil pass
			lightStencilPass(light, stencilShader);

			// Bind final texture as drawbuffer
			camera->getFramebuffer().bindDrawbuffers({ 4 });

			// Setup for point light pass
			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			// Point light pass
			pointLightPass(light, pointShader);
		}

		// Render spot lights
		for (SpotLight::Handle &light : manager->getWorld().entities.getAllComponents<SpotLight>(true))
		{
			// Create model matrix to scale sphere
			Transform::Handle transform = light.getEntity().getComponent<Transform>();
			glm::mat4 lightMatrix = glm::translate(transform->position) * glm::mat4_cast(transform->getRotation()) * glm::scale(glm::vec3(light->range));

			// Buffer uniform data
			glBindBuffer(GL_UNIFORM_BUFFER, transformBuffer.getBuffer());
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &lightMatrix[0][0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// Clear stencil buffer only
			camera->getFramebuffer().clear({}, false, true);

			// Setup for stencil pass
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glDisable(GL_CULL_FACE);
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			// Stencil pass
			lightStencilPass(light, stencilShader);

			// Bind final texture as drawbuffer
			camera->getFramebuffer().bindDrawbuffers({ 4 });

			// Setup for point light pass
			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			// Point light pass
			spotLightPass(light, spotShader);
		}

		// Renable backface culling and depth writing
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);

		// Disable blending and stencil test
		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST);
	}

	void Rendering::directionalPass(DirectionalLight::Handle &light, Shader &shader)
	{
		// Set uniforms
		shader.setUniform("light.direction", light->direction);
		shader.setUniform("light.color", light->color);
		shader.setUniform("light.intensity", light->intensity);
		shader.setUniform("light.ambient", light->ambient);
	
		// Draw light using full screen quad
		glBindVertexArray(renderingPrimatives["lightingQuad"]->vaObject);
		glDrawElements(GL_TRIANGLES, renderingPrimatives["lightingQuad"]->drawCount, GL_UNSIGNED_INT, 0);
	}

	void Rendering::lightStencilPass(PointLight::Handle &light, Shader &shader)
	{
		// Use shader
		shader.use();

		// Draw using scaled icosphere
		glBindVertexArray(renderingPrimatives["lightingSphere"]->vaObject);
		glDrawElements(GL_TRIANGLES, renderingPrimatives["lightingSphere"]->drawCount, GL_UNSIGNED_INT, 0);
	}

	void Rendering::lightStencilPass(SpotLight::Handle &light, Shader &shader)
	{
		// Use shader
		shader.use();

		// Draw using scaled icosphere
		glBindVertexArray(renderingPrimatives["lightingSphere"]->vaObject);
		glDrawElements(GL_TRIANGLES, renderingPrimatives["lightingSphere"]->drawCount, GL_UNSIGNED_INT, 0);
	}

	void Rendering::pointLightPass(PointLight::Handle &light, Shader &shader)
	{
		// Use shader
		shader.use();

		// Set uniforms
		shader.setUniform("positionIn", 0);
		shader.setUniform("diffuseOpacityIn", 1);
		shader.setUniform("normalIn", 2);
		shader.setUniform("screenSize", manager->getSystem<Display>()->getSize());
		shader.setUniform("light.position", light.getEntity().getComponent<Transform>()->position);
		shader.setUniform("light.color", light->color);
		shader.setUniform("light.intensity", light->intensity);
		shader.setUniform("light.ambient", light->ambient);
		shader.setUniform("light.radius", light->radius);
		shader.setUniform("light.falloff", light->falloff);

		// Draw using scaled icosphere
		glBindVertexArray(renderingPrimatives["lightingSphere"]->vaObject);
		glDrawElements(GL_TRIANGLES, renderingPrimatives["lightingSphere"]->drawCount, GL_UNSIGNED_INT, 0);
	}

	void Rendering::spotLightPass(SpotLight::Handle &light, Shader &shader)
	{
		// Use shader
		shader.use();

		// Set uniforms
		shader.setUniform("positionIn", 0);
		shader.setUniform("diffuseOpacityIn", 1);
		shader.setUniform("normalIn", 2);
		shader.setUniform("screenSize", manager->getSystem<Display>()->getSize());
		shader.setUniform("light.position", light.getEntity().getComponent<Transform>()->position);
		shader.setUniform("light.color", light->color);
		shader.setUniform("light.intensity", light->intensity);
		shader.setUniform("light.ambient", light->ambient);
		shader.setUniform("light.range", light->range);
		shader.setUniform("light.direction", glm::mat3(light.getEntity().getComponent<Transform>()->getModelMatrix()) * light->direction);
		shader.setUniform("light.innerAngle", cos(DegToRads(light->innerAngle)));
		shader.setUniform("light.outerAngle", cos(DegToRads(light->outerAngle)));

		// Draw using scaled icosphere
		glBindVertexArray(renderingPrimatives["lightingSphere"]->vaObject);
		glDrawElements(GL_TRIANGLES, renderingPrimatives["lightingSphere"]->drawCount, GL_UNSIGNED_INT, 0);
	}

	void Rendering::postProcessing(Camera::Handle &camera, unsigned int &passes)
	{
		// Disable depth testing
		glDisable(GL_DEPTH_TEST);

		// Enable alpha
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		// Loop through post processing effects and apply
		for (auto &effect : camera->getAllPostProcessingEffects())
		{
			if (effect.second->isEnabled())
			{
				// Load and use shader
				Shader &shader = effect.second->getShader();
				shader.use();

				// Set uniforms
				shader.setUniform("sceneTexture", 0);
				effect.second->setUniforms();

				// Bind framebuffer for post processing pass
				if (passes == 0)
				{
					// Read from final texture if using lighting, otherwise read from diffuse
					if (camera->getLighting())
						camera->getFramebuffer().bindTextures({ 4 });
					else
						camera->getFramebuffer().bindTextures({ 1 });

					// Draw to first post processing texture
					camera->getFramebuffer().bindDrawbuffers({ 5 });
				}
				else if (passes % 2 == 0)
				{
					// Even pass read from second post processing texture
					camera->getFramebuffer().bindTextures({ 6 });

					// And draw to first
					camera->getFramebuffer().bindDrawbuffers({ 5 });
				}
				else
				{
					// Odd pass read from first post processing texture
					camera->getFramebuffer().bindTextures({ 5 });

					// And draw to second
					camera->getFramebuffer().bindDrawbuffers({ 6 });
				}

				// Clear
				glClear(GL_COLOR_BUFFER_BIT);

				// Draw fullscreen quad
				glBindVertexArray(renderingPrimatives["framebufferQuad"]->vaObject);
				glDrawElements(GL_TRIANGLES, renderingPrimatives["framebufferQuad"]->drawCount, GL_UNSIGNED_INT, 0);

				// Increment pass count
				++passes;
			}
		}

		// Disable blending, renable depth test
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void Rendering::finalPass(Camera::Handle &camera, const unsigned int &postProcessingPasses)
	{
		// Disable depth test
		glDisable(GL_DEPTH_TEST);

		// Get handle to file system
		Files::Handle files = manager->getSystem<Files>();

		// Load and use shader
		Shader &shader = files->loadFile<Shader>("shaders/Post Processing/final_pass");
		shader.use();

		// Bind default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Bind textures for reading
		if (postProcessingPasses == 0 && camera->getLighting())
			camera->getFramebuffer().bindTextures({ 4 }); // Read from final texture
		else if (postProcessingPasses == 0)
			camera->getFramebuffer().bindTextures({ 1 }); // Read from diffuse texture
		else if (postProcessingPasses % 2 == 0)
			camera->getFramebuffer().bindTextures({ 6 }); // Read from 2nd post processing texture
		else
			camera->getFramebuffer().bindTextures({ 5 }); // Read from 1st post processing texture

		// Get camera position and size onscreen
		glm::vec2 position = camera->getPosition();
		glm::vec2 size = camera->getSize();

		// Calculate framebuffer matrix
		glm::mat4 framebufferMatrix = glm::translate(glm::vec3(position.x * (1.0f - size.x), position.y * (1.0f - size.y), 0.0f)) * glm::scale(glm::vec3(size, 0.0f));

		// Set uniforms
		shader.setUniform("framebufferMatrix", framebufferMatrix);
		shader.setUniform("camera", 0);

		// Enable blending if needed
		if (camera->getBlend())
		{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_SUBTRACT);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		// Draw final texture to screen
		glBindVertexArray(renderingPrimatives["framebufferQuad"]->vaObject);
		glDrawElements(GL_TRIANGLES, renderingPrimatives["framebufferQuad"]->drawCount, GL_UNSIGNED_INT, 0);

		// Renable depth test and disable blending
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	UniformBuffer &Rendering::newUniformBuffer(const std::string &name, unsigned int size)
	{
		// Generate new uniform buffer
		uniformBuffers[name] = std::make_unique<UniformBuffer>(name, size, uniformBuffers.size() + 1);
		return *uniformBuffers[name];
	}

	bool Rendering::uniformBufferExists(const std::string &name)
	{
		return uniformBuffers.find(name) != uniformBuffers.end();
	}

	UniformBuffer &Rendering::getUniformBuffer(const std::string &name)
	{
		assert(uniformBuffers.find(name) != uniformBuffers.end());
		return *uniformBuffers[name];
	}
}