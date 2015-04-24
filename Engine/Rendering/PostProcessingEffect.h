#ifndef _POSTPROCESSINGEFFECT_H
#define _POSTPROCESSINGEFFECT_H

#include "Shader.h"
#include "../Core/Files.h"

namespace Engine
{
	class BasePostProcessingEffect
	{
		friend class Camera;
	public:
		typedef std::shared_ptr<BasePostProcessingEffect> Ptr;

		BasePostProcessingEffect(const std::string &shaderName);
		virtual ~BasePostProcessingEffect() { }
		virtual void setup() {}
		virtual void setUniforms() {}

		Shader &getShader();
		void enable();
		void disable();
		bool isEnabled();
		bool bindDepth();

	protected:
		std::string shaderName;
		Shader shader;
		Files::Handle files;
		bool enabled = false;
		bool depthTexture = false;

	private:
		void loadShader();
	};

	template <typename T>
	class PostProcessingEffect : public BasePostProcessingEffect
	{
	public:
		typedef std::shared_ptr<T> Ptr;

		PostProcessingEffect(const std::string &shaderName) : BasePostProcessingEffect(shaderName) { }
		virtual ~PostProcessingEffect() { }

		static EntitySystem::TypeId getTypeId()
		{
			return EntitySystem::Type<BasePostProcessingEffect>::getTypeId<T>();
		}
	};
}

#endif