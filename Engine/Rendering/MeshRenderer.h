#ifndef _MESHRENDERER_H
#define _MESHRENDERER_H

#include <string>

#include "../EntitySystem/Entity.h"

namespace Engine
{
	class MeshRenderer : public EntitySystem::Component<MeshRenderer>
	{
		friend class Rendering;
	public:
		MeshRenderer(const std::string &file);

		virtual void initialise();
		virtual void uninitialise();

	private:
		std::string file;
	};
}

#endif