#pragma once

#include "Triton\TRMacros.h" 
#include "glm\glm.hpp"

namespace Triton
{
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	typedef glm::mat4 Matrix44;

	namespace Core
	{
		TRITON_API Matrix44 CreateProjectionMatrix(float aWindowWidth, float aWindowHeight, float aFoV, float aFarRenderPlane, float aNearRenderPlane);
		TRITON_API Matrix44 CreateTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale);
	}
}