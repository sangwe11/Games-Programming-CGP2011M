#include "Material2D.h"
#include "Texture2D.h"

namespace Engine
{
	bool Material2D::load(aiMaterial *material, const std::string &modelPath)
	{
		// Retrieve material names
		material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
		material->Get(AI_MATKEY_SHININESS, shininess);
		material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
		material->Get(AI_MATKEY_OPACITY, opacity);

		// Ambient texture
		if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_AMBIENT, 0, &path);

			// Load texture
			Texture2D &texture = files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[TextureType::Ambient] = &texture;
		}

		// Diffuse texture
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			// Load texture
			Texture2D &texture = files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[TextureType::Diffuse] = &texture;
		}

		// Normals texture
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_NORMALS, 0, &path);

			// Load texture
			Texture2D &texture = files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[TextureType::Normal] = &texture;
		}

		// Height texture as normals
		if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_HEIGHT, 0, &path);

			// Load texture
			Texture2D &texture = files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[TextureType::Normal] = &texture;
		}

		return true;
	}

	void Material2D::use(Shader &shader)
	{
		// Update material uniforms
		shader.setUniform("diffuseColor", diffuse);
		shader.setUniform("opacity", opacity);
		shader.setUniform("specularColor", specular);
		shader.setUniform("specularExponent", shininess);
		shader.setUniform("hasDiffuseMap", (textures[Material2D::TextureType::Diffuse] != nullptr));
		shader.setUniform("hasNormalMap", (textures[Material2D::TextureType::Normal] != nullptr));

		// Use diffuse texture
		if (textures[Material2D::TextureType::Diffuse] != nullptr)
		{
			// Bind to texture unit 0
			textures[Material2D::TextureType::Diffuse]->use(0);

			// Set shader uniform
			shader.setUniform("diffuseMap", 0);
		}

		// Use normal texture
		if (textures[Material2D::TextureType::Normal] != nullptr)
		{
			// Bind to texture unit 1
			textures[Material2D::TextureType::Normal]->use(1);

			// Set shader uniform
			shader.setUniform("normalMap", 1);
		}
	}
}