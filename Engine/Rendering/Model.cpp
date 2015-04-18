#include "Model.h"
#include "Mesh.h"
#include "Material2D.h"

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/scene.h>

namespace Engine
{
	bool Model::load()
	{
		// ASSIMP for model loading provides greater support
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(name, aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_TransformUVCoords | aiProcess_OptimizeMeshes | aiProcess_RemoveRedundantMaterials);

		if (!scene)
		{
			// Couldn't load file
			std::cout << importer.GetErrorString() << std::endl;
			return false;
		}

		// Work out the model path
		std::string modelPath = name.substr(0, name.find_last_of("\\/"));
		modelPath.append("/");

		// Load meshes
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			// Assimp doesn't seem to load names very well..
			std::string meshName = name + "_mesh_" + std::to_string(i);

			// Load mesh
			files->loadFile<Mesh>(meshName, scene->mMeshes[i]);

			// Store mesh name
			meshes.push_back(meshName);
		}

		// Load materials
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			// Assimp doesn't seem to load names very well..
			std::string materialName = name + "_material2D_" + std::to_string(i);

			// Load material
			files->loadFile<Material2D>(materialName, scene->mMaterials[i], modelPath);

			// Store material name
			materials.push_back(materialName);
		}

		// Free scene
		importer.FreeScene();

		return true;
	}

	void Model::cleanup()
	{
		// Clear lists
		meshes.clear();
		materials.clear();
	}
}