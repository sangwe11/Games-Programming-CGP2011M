#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Core/Maths.h"
#include "../EntitySystem/Entity.h"
#include "Framebuffer.h"
#include "PostProcessingEffect.h"

namespace Engine
{
	class Camera : public EntitySystem::Component<Camera>
	{
	public:
		Camera(const float &fov, const float &zNear, const float &zFar, const bool &lighting, const bool &blend, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &clearColor);

		virtual void initialise();

		void resize(const unsigned int &width, const unsigned int &height);

		const glm::mat4 getView();

		const glm::mat4 &getProjection() const { return projection; }
		const float &getFov() const { return fov; }
		const float &getZNear() const { return zNear; }
		const float &getZFar() const { return zFar; }
		const bool &getLighting() const { return lighting; }
		const bool &getBlend() const { return blend; }
		const glm::vec2 &getSize() const { return size; }
		const glm::vec2 &getPosition() const { return position; }
		const glm::vec4 &getClearColor() const { return clearColor; }

		Framebuffer &getFramebuffer() { return *framebuffer; }
	
		template <typename T, typename... Args>
		typename T &addPostProcessingEffect(Args ... args)
		{
			// Check if effect of type already exists
			if (postProcessingEffects[T::getTypeId()] != nullptr)
			{
				// Return the existing effect
				std::cout << "Post Processing effect of this type has already been added." << std::endl;
				return dynamic_cast<T &>(*postProcessingEffects[T::getTypeId()]);
			}

			// Create the effect
			postProcessingEffects[T::getTypeId()] = std::make_unique<T>(std::forward<Args>(args) ...);

			// Setup
			postProcessingEffects[T::getTypeId()]->files = manager->getWorld().systems.getSystem<Files>();
			postProcessingEffects[T::getTypeId()]->loadShader();
			postProcessingEffects[T::getTypeId()]->setup();

			return dynamic_cast<T &>(*postProcessingEffects[T::getTypeId()]);
		}

		template <typename T>
		typename T &getPostProcessingEffect()
		{
			// Assert the effect exists
			assert(postProcessingEffects[T::getTypeId()] != nullptr);

			// Return existing effect
			return dynamic_cast<T &>(*postProcessingEffects[T::getTypeId()]);
		}

		const std::map<EntitySystem::TypeId, std::unique_ptr<BasePostProcessingEffect>> &getAllPostProcessingEffects() { return postProcessingEffects; }

		template <typename T>
		void removePostProcessingEffect()
		{
			// Remove effect from list
			postProcessingEffects.erase(T::getTypeId());
		}

	private:
		float fov;
		float zNear;
		float zFar;
		glm::mat4 projection;

		bool lighting;
		bool blend;
		glm::vec2 position;
		glm::vec2 size;	
		glm::vec4 clearColor;

		std::unique_ptr<Framebuffer> framebuffer;

		std::map<EntitySystem::TypeId, std::unique_ptr<BasePostProcessingEffect>> postProcessingEffects;
	};
}

#endif