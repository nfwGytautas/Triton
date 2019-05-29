#pragma once

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN


inline void DXRenderer::create()
{
	
}

inline void Triton::PType::DXRenderer::render(PlatformObject* renderable)
{
	auto glRenderable = OBJECT_AS(DXVAO, renderable);

	if (glRenderable != nullptr)
	{
		m_deviceContext->DrawIndexed(glRenderable->m_indiceCount, 0, 0);
	}
	else
	{
		TR_CORE_ERROR("Passed PlatformObject is not of type DXRenderable.");
	}
}

inline void Triton::PType::DXRenderer::default()
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
}

inline Matrix44 Triton::PType::DXRenderer::projection()
{
	return m_projectionMatrix;
}

inline Matrix44 Triton::PType::DXRenderer::recalc_projection()
{
	m_projectionMatrix = Triton::Core::CreateProjectionMatrix(m_width, m_height, fov, nearPlane, farPlane);
	return m_projectionMatrix;
}

inline void Triton::PType::DXRenderer::newFrame(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

inline void Triton::PType::DXRenderer::endFrame()
{
	

	return;
}


NAMESPACE_END