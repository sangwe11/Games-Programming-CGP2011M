#include "MinimapController.h"
#include "Core/Engine.h"
#include "Config.h"
#include <GLM/gtx/matrix_interpolation.hpp>

MinimapController::MinimapController(Entity &player, glm::vec3 offset) : player(player), offset(offset)
{
}

void MinimapController::Init()
{
	// Require a transform component
	engine->RequireComponentOnEntity<Transform>(entity);
	engine->RequireComponentOnEntity<Transform>(player);

	// Grab and store the transform component
	transform = engine->GetComponentFromEntity<Transform>(entity);
	playerTransform = engine->GetComponentFromEntity<Transform>(player);
}

void MinimapController::LateUpdate()
{
	if (std::shared_ptr<Transform> playerTransformPtr = playerTransform.lock())
	{
		// Camera is always postioned at the players position + offset
		transform->position = playerTransformPtr->position + offset;

		// Keep camera rotated in the direction of the player to keep players forward direction up on the map
		transform->RotateGlobal(playerTransformPtr->eulerAngles.y - transform->eulerAngles.y, glm::vec3(0, 1, 0));
	}
}