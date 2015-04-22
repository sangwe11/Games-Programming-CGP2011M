#ifndef _MODEL_H
#define _MODEL_H

#include "../Core/Files.h"
#include "Mesh.h"
#include "Material2D.h"
#include "Shader.h"

namespace Engine
{
	class Model : public File<Model>
	{
	public:
		virtual bool load();
		virtual void cleanup();

		void draw(Shader &shader);

	private:
		std::vector<Mesh *> meshes;
		std::vector<Material2D *> materials;
	};
}

#endif