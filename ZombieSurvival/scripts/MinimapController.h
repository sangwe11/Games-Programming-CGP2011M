#ifndef _MINIMAPCONTROLLER_H
#define _MINIMAPCONTROLLER_H

#include "Scripting/Script.h"
#include "Core/Transform.h"

class MinimapController : public Script
{
public:
	MinimapController(Entity &player, glm::vec3 offset);

	void Init();
	void LateUpdate();

private:
	glm::vec3 offset;
	Entity &player;
	std::shared_ptr<Transform> transform;
	std::weak_ptr<Transform> playerTransform;
};

#endif
