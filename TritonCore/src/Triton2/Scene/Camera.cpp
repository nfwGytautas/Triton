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
	{
		m_viewMatrix = Core::lookAt(position, targetPosition);
	}
}