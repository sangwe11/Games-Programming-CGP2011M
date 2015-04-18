#ifndef _MATERIAL2D_H
#define _MATERIAL2D_H

#include <string>
#include <ASSIMP/material.h>
#include <GLM/glm.hpp>

#include "../Core/Files.h"

namespace Engine
{
	class Material2D : public Engine::File<Material2D>
	{
	public:
		virtual bool load(aiMaterial *material, const std::string &modelPath);

	private:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emissive;
		float shininess;
		float shininessStrength;
		float opacity;

		std::string textures[10];
	};
}

#endif