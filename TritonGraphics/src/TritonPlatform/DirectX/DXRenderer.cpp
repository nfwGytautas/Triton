#include "DXRenderer.h"

#include "BasicTypes/DXVAO.h"
#include "BasicTypes/DXBitmap.h"

PLATFORM_NAMESPACE_BEGIN

void DXRenderer::create()
{

}

void Triton::PType::DXRenderer::render(reference<Renderable>& renderable)
{
	m_deviceContext->DrawIndexed(renderable->getIndiceCount(), 0, 0);
}

void DXRenderer::render(unsigned int indiceCount)
{
	m_deviceContext->DrawIndexed(indiceCount, 0, 0);
}

void Triton::PType::DXRenderer::default()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
}

Matrix44 Triton::PType::DXRenderer::projection()
{
	return m_projectionMatrix;
}

Matrix44 Triton::PType::DXRenderer::orthographic()
{
	return m_orthographicMatrix;
}

void Triton::PType::DXRenderer::recalc_matrices()
{
	m_projectionMatrix = Triton::Core::CreateProjectionMatrix(width, height, fov, nearPlane, farPlane);
	m_orthographicMatrix = Triton::Core::CreateOrthographicMatrix(width, height, nearPlane, farPlane);
}

void Triton::PType::DXRenderer::setVsync(bool value)
{
	m_vsync = value;
}

void Triton::PType::DXRenderer::newFrame(float red, float green, float blue, float alpha)
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

void Triton::PType::DXRenderer::endFrame()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}

PLATFORM_NAMESPACE_END