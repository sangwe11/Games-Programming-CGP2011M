#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "../EntitySystem/Entity.h"
#include "Maths.h"

namespace Engine
{
	class Transform : public EntitySystem::Component<Transform>
	{
	public:
		Transform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);

		const glm::mat4 getModelMatrix() const;
		const glm::quat &getRotation() const;
		const glm::vec3 &getEulers() const;
		const glm::vec3 &getXAxis() const;
		const glm::vec3 &getYAxis() const;
		const glm::vec3 &getZAxis() const;

		const glm::vec3 forward() { return -zaxis; }
		const glm::vec3 right() { return xaxis; }
		const glm::vec3 up() { return yaxis; }

		void rotateLocal(const float &amount, const glm::vec3 &axis);
		void rotateGlobal(const float &amount, const glm::vec3 &axis);

		glm::vec3 position;
		glm::vec3 scale;

	private:
		glm::quat rotation;
		glm::vec3 eulerAngles;
		glm::vec3 xaxis;
		glm::vec3 yaxis;
		glm::vec3 zaxis;

		void calculateAxis();
	};
}

#endif