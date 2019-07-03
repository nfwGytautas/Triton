#pragma once

#include "TritonPlatform/mathematical.h"

namespace Triton
{

	class Camera
	{
	public:
		Camera(Vector3 aPosition, float aMoveSpeed = 3.0f, float aLookSpeed = 0.005f);
		~Camera();

		Vector3 Position;

		Vector3 GetViewDirection() const;

		float Pitch = 0.0f;
		float Yaw = 0.0f;

		void OnUpdate();
		Matrix44& ViewMatrix();
	private:
		Matrix44 m_ViewMatrix;

		Vector3 m_ViewDirection;
		Vector3 viewPoint;
		Vector3 right;
		
		float m_moveSpeed = 3.0f;
		float m_lookSpeed = 0.005f;
	};

}