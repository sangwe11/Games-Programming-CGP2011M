#ifndef _MATHS_H
#define _MATHS_H

#define GLM_FORCE_RADIANS
#define PI 3.14159265359f

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/rotate_vector.hpp>

namespace Engine
{
	/*! \fn DegToRads(T degrees)
	\brief Converts from degrees to radians.
	\param degrees units to convert to radians, in degrees.
	*/
	template <typename T>
	T DegToRads(T degrees)
	{
		return (degrees * PI) / 180.0f;
	}

	/*! \fn RadToDegs(T radians)
	\brief Converts from radians to degrees.
	\param radians units to convert to degrees, in radians.
	*/
	template <typename T>
	T RadToDegs(T radians)
	{
		return (radians * 180.0f) / PI;
	}
}

#endif