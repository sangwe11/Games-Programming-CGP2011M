#include "Skybox.h"
#include "../Core/Files.h"
#include "../EntitySystem/World.h"

namespace Engine
{
	Skybox::Skybox(const std::string &directory, const std::string &positiveX, const std::string &negativeX, const std::string &positiveY, const std::string &negativeY, const std::string &positiveZ, const std::string &negativeZ)
	{
		// Store directory
		this->directory = directory;

		// Store texture names
		textureFiles.emplace_back(positiveX);
		textureFiles.emplace_back(negativeX);
		textureFiles.emplace_back(positiveY);
		textureFiles.emplace_back(negativeY);
		textureFiles.emplace_back(positiveZ);
		textureFiles.emplace_back(negativeZ);
	}

	void Skybox::initialise()
	{
		// Load skybox into 3d texture
		texture = manager->getWorld().systems.getSystem<Files>()->loadFile<Texture3D>(directory, textureFiles[0], textureFiles[1], textureFiles[2], textureFiles[3], textureFiles[4], textureFiles[5]);
	}
}