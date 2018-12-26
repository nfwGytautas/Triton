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

	Vector3 viewDirection = glm::vec3(
		cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)),
		sin(glm::radians(Pitch)),
		cos(glm::radians(Pitch)) * sin(glm::radians(Yaw))
	);

	m_ViewDirection = glm::normalize(viewDirection);
	viewPoint = Position + m_ViewDirection;
	Vector3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 ViewMatrix = glm::lookAt(
		Position,
		viewPoint,
		up
	);

	m_ViewMatrix = ViewMatrix;
}

const Triton::Matrix44& Triton::Camera::ViewMatrix()
{
	return m_ViewMatrix;
}
