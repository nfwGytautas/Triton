#include "TRpch.h"
#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

Triton::Camera::Camera(Vector3 aPosition, float aMoveSpeed, float aLookSpeed)
	:Position(aPosition), m_moveSpeed(aMoveSpeed), m_lookSpeed(aLookSpeed)
{
}

Triton::Camera::~Camera()
{
}

Triton::Vector3 Triton::Camera::GetViewDirection() const
{
	return m_ViewDirection;
}

void Triton::Camera::OnUpdate()
{
	if (Pitch > 89.99999f)
		Pitch = 89.99999f;
	if (Pitch < -89.99999f)
		Pitch = -89.99999f;
	
	m_ViewMatrix = Triton::Core::createViewMatrix(Position, Triton::to_radians(Pitch), Triton::to_radians(Yaw), Triton::to_radians(0.0f));
}

Triton::Matrix44& Triton::Camera::ViewMatrix()
{
	return m_ViewMatrix;
}
