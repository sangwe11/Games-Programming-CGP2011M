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
			files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[0] = path.C_Str();
		}

		// Diffuse texture
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			// Load texture
			files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[1] = path.C_Str();
		}

		// Normals texture
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_NORMALS, 0, &path);

			// Load texture
			files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[2] = path.C_Str();
		}

		// Height texture as normals
		if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			// Get texture path
			aiString path;
			material->GetTexture(aiTextureType_HEIGHT, 0, &path);

			// Load texture
			files->loadFile<Texture2D>(modelPath + path.C_Str(), true, true, 16);
			textures[2] = path.C_Str();
		}

		return true;
	}
}