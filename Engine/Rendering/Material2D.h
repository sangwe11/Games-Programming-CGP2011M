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
		enum TextureType { Ambient, Diffuse, Normal, Height, Specular, Emissive };

		virtual bool load(aiMaterial *material, const std::string &modelPath);

		void use(Shader &shader);

	private:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emissive;
		float shininess;
		float shininessStrength;
		float opacity;

		Texture2D *textures[10];
	};
}

#endif