#pragma once

#ifdef TR_PLATFORM_WINDOWS

#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
namespace Triton
{
	typedef DirectX::SimpleMath::Vector2 Vector2;
	typedef DirectX::SimpleMath::Vector3 Vector3;
	typedef DirectX::SimpleMath::Vector4 Vector4;

	typedef DirectX::SimpleMath::Matrix Matrix44;

	namespace Core
	{
		Matrix44 CreateProjectionMatrix(float aWindowWidth, float aWindowHeight, float aFoV, float aNearRenderPlane, float aFarRenderPlane);
		Matrix44 CreateTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale);
		Matrix44 createViewMatrix(Vector3 position, float pitch, float yaw, float roll);
		Matrix44 CreateOrthographicMatrix(float aWindowWidth, float aWindowHeight, float aNearRenderPlane, float aFarRenderPlane);

		Matrix44 transpose(Matrix44& mat);
	}
}

#else

#include "glm\glm.hpp"

namespace Triton
{
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	typedef glm::mat4 Matrix44;

	namespace Core
	{
		Matrix44 CreateProjectionMatrix(float aWindowWidth, float aWindowHeight, float aFoV, float aNearRenderPlane, float aFarRenderPlane);
		Matrix44 CreateTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale);
	}
}

#endif

namespace Triton
{
	inline float to_radians(float degrees);
}