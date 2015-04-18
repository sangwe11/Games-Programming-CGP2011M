#ifndef _MODEL_H
#define _MODEL_H

#include "../Core/Files.h"

namespace Engine
{
	class Model : public File<Model>
	{
	public:
		virtual bool load();
		virtual void cleanup();

		std::vector<std::string> meshes;
		std::vector<std::string> materials;
	};
}

#endif