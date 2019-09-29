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
			auto[width, height] = m_renderingTo->size();
			m_projectionMatrix = Core::CreateProjectionMatrix(width, height, m_fov, m_nearPlane, m_farPlane);
			m_orthographicMatrix = Core::CreateOrthographicMatrix(width, height, m_nearPlane, m_farPlane);
		}
	}
}