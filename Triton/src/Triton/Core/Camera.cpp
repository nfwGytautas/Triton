#include "TRpch.h"
#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

Triton::Camera::Camera(Vector3 aPosition, Vector3 aTarget, float aMoveSpeed, float aLookSpeed)
	:Position(aPosition), Target(aTarget), m_moveSpeed(aMoveSpeed), m_lookSpeed(aLookSpeed)
{
}

Triton::Camera::~Camera()
{
}

void Triton::Camera::OnUpdate()
{
	Vector3 viewDirection = glm::vec3(
		cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)),
		sin(glm::radians(Pitch)),
		cos(glm::radians(Pitch)) * sin(glm::radians(Yaw))
	);

	ViewDirection = glm::normalize(viewDirection);
	viewPoint = Position + ViewDirection;
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
