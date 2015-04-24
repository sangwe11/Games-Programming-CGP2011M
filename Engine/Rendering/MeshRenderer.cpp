#include "MeshRenderer.h"
#include "Model.h"
#include "../Core/Files.h"
#include "../EntitySystem/World.h"

namespace Engine
{
	MeshRenderer::MeshRenderer(const std::string &file)
	{
		// Store file name
		this->file = file;
	}

	void MeshRenderer::initialise()
	{
		// Preload the model file
		Model &model = manager->getWorld().systems.getSystem<Files>()->loadFile<Model>(file);
	}

	void MeshRenderer::uninitialise()
	{
	}
}