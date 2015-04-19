#ifndef _MATERIAL2D_H
#define _MATERIAL2D_H

#include <string>
#include <ASSIMP/material.h>
#include <GLM/glm.hpp>

#include "../Core/Files.h"
#include "Texture2D.h"
#include "Shader.h"

namespace Engine
{
	class Material2D : public Engine::File<Material2D>
	{
		friend class Rendering;

	public:
		enum TextureType { Ambient, Diffuse, Normal, Height, Specular, Emissive, Count };

		Material2D();

		virtual bool load(aiMaterial *material, const std::string &modelPath);
		virtual void cleanup();

		void use(Shader &shader);

	private:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emissive;
		float shininess;
		float shininessStrength;
		float opacity;

		bool ambientTexture = false;
		bool diffuseTexture = false;
		bool normalTexture = false;

		Texture2D *textures[TextureType::Count];
	};
}

#endif