#include "Renderer.h"

#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	namespace Graphics
	{
		bool Renderer::valid() const
		{
			return m_initialized;
		}

		void Renderer::vsync(bool value)
		{
			m_vsync = value;
		}

		void Renderer::fov(float value)
		{
			m_fov = value;
			recalcMatrices();
		}

		void Renderer::nearPlane(float value)
		{
			m_nearPlane = value;
			recalcMatrices();
		}

		void Renderer::farPlane(float value)
		{
			m_farPlane = value;
			recalcMatrices();
		}

		Matrix44 Renderer::projection()
		{
			return m_projectionMatrix;
		}

		Matrix44 Renderer::orthographic()
		{
			return m_orthographicMatrix;
		}

		void Renderer::recalcMatrices()
		{
			m_projectionMatrix = Core::CreateProjectionMatrix(m_surfaceWidth, m_surfaceHeight, m_fov, m_nearPlane, m_farPlane);
			m_orthographicMatrix = Core::CreateOrthographicMatrix(m_surfaceWidth, m_surfaceHeight, m_nearPlane, m_farPlane);
		}
	}
}