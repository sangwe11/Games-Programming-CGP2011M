#include "Transform.h"

#include <GLM/ext.hpp>

namespace Engine
{
	Transform::Transform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
	{
		this->position = position;
		this->scale = scale;

		// Store euler angles
		this->eulerAngles = eulerAngles;

		// Rotate into position
		rotateLocal(eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotateLocal(eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotateLocal(eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	const glm::mat4 Transform::getModelMatrix() const
	{
		glm::mat4 positionMatrix = glm::translate(position);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(scale);

		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	const glm::quat &Transform::getRotation() const
	{
		return rotation;
	}

	const glm::vec3 &Transform::getEulers() const
	{
		return eulerAngles;
	}

	const glm::vec3 &Transform::getXAxis() const
	{
		return xaxis;
	}

	const glm::vec3 &Transform::getYAxis() const
	{
		return yaxis;
	}

	const glm::vec3 &Transform::getZAxis() const
	{
		return zaxis;
	}

	void Transform::rotateLocal(const float &amount, const glm::vec3 &axis)
	{
		rotation = rotation * glm::angleAxis(amount, axis);

		// Recalculate local axis
		calculateAxis();
	}

	void Transform::rotateGlobal(const float &amount, const glm::vec3 &axis)
	{
		glm::quat rotationQuat = glm::normalize(glm::angleAxis(amount, axis));
		glm::mat3 rotationMatrix = glm::mat3_cast(rotationQuat);

		// Rotate euler angles
		glm::quat q = rotationQuat;
		eulerAngles.x += RadToDegs(atan2(2.0f*(q.y*q.z + q.w*q.x), q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z));
		eulerAngles.y += RadToDegs(asin(-2.0f*(q.x*q.z - q.w*q.y)));
		eulerAngles.z += RadToDegs(atan2(2.0f*(q.x*q.y + q.w*q.z), q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z));

		// Rotate stored orientation
		rotation = rotationQuat * rotation;

		// Recalculate local axis
		calculateAxis();
	}

	void Transform::calculateAxis()
	{
		glm::mat3 view = glm::mat3_cast(rotation);

		// Local axis
		xaxis = glm::normalize(view * glm::vec3(1.0f, 0.0f, 0.0f));
		yaxis = glm::normalize(view * glm::vec3(0.0f, 1.0f, 0.0f));
		zaxis = glm::normalize(view * glm::vec3(0.0f, 0.0f, 1.0f));

	}
}