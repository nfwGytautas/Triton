#include "DXFramebuffer.h"

namespace Triton
{
	namespace Graphics
	{

		DXFramebuffer::~DXFramebuffer()
		{
			if (m_shaderResourceView)
			{
				m_shaderResourceView->Release();
				m_shaderResourceView = 0;
			}

			if (m_renderTargetView)
			{
				m_renderTargetView->Release();
				m_renderTargetView = 0;
			}

			if (m_renderTargetTexture)
			{
				m_renderTargetTexture->Release();
				m_renderTargetTexture = 0;
			}
		}

		void DXFramebuffer::enable()
		{
			m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		}

		void DXFramebuffer::disable()
		{

		}

		void DXFramebuffer::clear(float red, float green, float blue, float alpha)
		{
			float color[4];

			// Setup the color to clear the buffer to.
			color[0] = red;
			color[1] = green;
			color[2] = blue;
			color[3] = alpha;

			m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

			// Clear the back buffer.
			m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

			// Clear the depth buffer.
			m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			return;
		}

		void DXFramebuffer::render()
		{

		}

	}
}