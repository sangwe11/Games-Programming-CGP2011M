#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "../EntitySystem/Entity.h"
#include "Texture3D.h"

namespace Engine
{
	class Skybox : public EntitySystem::Component<Skybox>
	{
		friend class Rendering;

	public:
		Skybox(const std::string &directory, const std::string &positiveX, const std::string &negativeX, const std::string &positiveY, const std::string &negativeY, const std::string &positiveZ, const std::string &negativeZ);

		virtual void initialise();

	private:
		Texture3D texture;
		std::string directory;
		std::vector<std::string> textureFiles;

	};
}

#endif