#include "Camera.h"
#include "../EntitySystem/World.h"
#include "Display.h"
#include "../Core/Transform.h"

#include <GLM/gtc/matrix_transform.hpp>

namespace Engine
{
	Camera::Camera(const float &fov, const float &zNear, const float &zFar, const bool &lighting, const bool &blend, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &clearColor)
	{
		this->fov = fov;
		this->zNear = zNear;
		this->zFar = zFar;
		this->lighting = lighting;
		this->blend = blend;
		this->position = glm::clamp(position, glm::vec2(-1.0f), glm::vec2(1.0f));
		this->size = glm::clamp(size, glm::vec2(0.0f), glm::vec2(1.0f));
		this->clearColor = clearColor;
	}

	void Camera::initialise()
	{
		// Get a reference to the display
		Display::Handle display = manager->getWorld().systems.getSystem<Display>();

		// Store projection matrix
		projection = glm::perspective(DegToRads(fov), display->getAspectRatio(), zNear, zFar);

		const unsigned int width = display->getWidth();
		const unsigned int height = display->getHeight();

		// Create a new framebuffer with 5 textures and depth
		// Textures: Diffuse, Position, Normal, Specular, Final
		framebuffer = std::make_unique<Framebuffer>(width, height, 5, true);
	}

	void Camera::resize(const unsigned int &width, const unsigned int &height)
	{
		// Recalculate projection matrix
		projection = glm::perspective(DegToRads(fov), (float)width / (float)height, zNear, zFar);

		// Create a new framebuffer with 5 textures and depth
		// Textures: Diffuse, Position, Normal, Specular, Final
		framebuffer = std::make_unique<Framebuffer>(width, height, 5, true);
	}

	const glm::mat4 Camera::getView()
	{
		return glm::inverse(entity.getComponent<Transform>()->getModelMatrix());
	}
}