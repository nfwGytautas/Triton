#pragma once

#include "Math/Math.h"

namespace Triton
{

	class TRITON_API Camera
	{
	public:
		Camera(Vector3 aPosition, Vector3 aTarget, float aMoveSpeed = 3.0f, float aLookSpeed = 0.005f);
		~Camera();

		Vector3 Position;
		Vector3 Target;
		Vector3 ViewDirection;

		float Pitch = 0.0f;
		float Yaw = 0.0f;

		void OnUpdate();
		const Matrix44& ViewMatrix();
	private:
		Matrix44 m_ViewMatrix;

		Vector3 viewPoint;
		Vector3 right;
		
		float m_moveSpeed = 3.0f;
		float m_lookSpeed = 0.005f;
	};

}