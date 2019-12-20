#pragma once

#ifdef TR_PLATFORM_WINDOWS

#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

namespace DirectX
{
	namespace SimpleMath
	{
		template<class Archive>
		void serialize(Archive& archive,
			Vector2& m)
		{
			archive(m.x, m.y);
		}

		template<class Archive>
		void serialize(Archive& archive,
			Vector3& m)
		{
			archive(m.x, m.y, m.z);
		}

		template<class Archive>
		void serialize(Archive& archive,
			Vector4& m)
		{
			archive(m.x, m.y, m.z, m.w);
		}
	}
}

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

		Matrix44 lookAt(Vector3 cameraPosition, Vector3 target);

		Matrix44 transpose(Matrix44& mat);

		Vector2 normalize(Vector2 vec);
		Vector3 normalize(Vector3 vec);
		Vector4 normalize(Vector4 vec);
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