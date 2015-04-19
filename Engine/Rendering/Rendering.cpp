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
		renderingPrimatives["framebufferQuad"] = std::make_unique<RenderingQuad>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Post Processing/final_pass", "shaders/Post Processing/final_pass.vertex", "shaders/Post Processing/final_pass.fragment"));
		renderingPrimatives["lightingQuad"] = std::make_unique<RenderingQuad>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Lighting/lighting_directional", "shaders/Lighting/lighting_directional.vertex", "shaders/Lighting/lighting_directional.fragment"));
		renderingPrimatives["skyboxCube"] = std::make_unique<RenderingCube>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Geometry/skybox", "shaders/Geometry/skybox.vertex", "shaders/Geometry/skybox.fragment"));
		renderingPrimatives["lightingSphere"] = std::make_unique<RenderingSphere>(manager->getSystem<Files>()->loadFile<Shader>("shaders/Lighting/lighting_point", "shaders/Lighting/lighting_point.vertex", "shaders/Lighting/lighting_point.fragment"), 2);
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
			// Get the cameras framebuffer
			Framebuffer &framebuffer = camera->getFramebuffer();

			// Buffer uniform data
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer.getBuffer());
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera->getView()[0][0]);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camera->getProjection()[0][0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// Clear textures and depth ready for drawing (Diffuse, Position, Normal and Specular)
			framebuffer.clear({ 0, 1, 2, 3 }, true);

			// Render geometry
			meshRender(camera);

			// Render skybox
			if (camera.getEntity().hasComponent<Skybox>())
				skyboxRender(camera);

			// Render camera to screen
			finalPass(camera);

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
		Shader &shader = files->loadFile<Shader>("mesh_render", "shaders/Geometry/mesh_render.vertex", "shaders/Geometry/mesh_render.fragment");
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
		Shader &shader = files->loadFile<Shader>("shaders/Geometry/skybox", "shaders/Geometry/skybox.vertex", "shaders/Geometry/skybox.fragment");
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

	void Rendering::finalPass(Camera::Handle &camera)
	{
		// Disable depth test
		glDisable(GL_DEPTH_TEST);

		// Get handle to file system
		Files::Handle files = manager->getSystem<Files>();

		// Load and use shader
		Shader &shader = files->loadFile<Shader>("shaders/Post Processing/final_pass", "shaders/Post Processing/final_pass.vertex", "shaders/Post Processing/final_pass.fragment");
		shader.use();

		// Bind default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Bind textures for reading
		if (camera->getLighting())
			camera->getFramebuffer().bindTextures({ 4 });
		else
			camera->getFramebuffer().bindTextures({ 1 });

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