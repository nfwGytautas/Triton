#include "TRpch.h"
#include "Camera.h"

namespace Triton
{
	const Matrix44& Camera::viewMatrix() const
	{
		return m_viewMatrix;
	}

	Camera::Camera()
	{
	}

	StaticCamera::StaticCamera(Vector3 position, Vector3 targetPosition)
		: m_Position(position), m_Target(targetPosition)
	{
		m_viewMatrix = Core::lookAt(position, targetPosition);
	}
	Vector3 StaticCamera::getPosition() const
	{
		return m_Position;
	}
	Vector3 StaticCamera::getViewDirection() const
	{
		Vector3 dir = Vector3(
			m_Target.x - m_Position.x,
			m_Target.y - m_Position.y,
			m_Target.z - m_Position.z
			);

		return dir;
	}
}