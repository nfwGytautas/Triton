#pragma once

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN

inline void DXTexture::enable()
{
	// Set shader texture resource in the pixel shader.
	m_deviceContext->PSSetShaderResources(0, 1, &m_textureView);
}

inline void DXTexture::disable()
{
	
}

inline void DXTexture::create(FactoryCreateParams* createParams)
{
	auto texParams = OBJECT_AS(TextureCreateParams, createParams);

	
}

inline void DXTexture::destroy(FactoryDestroyParams* destroyParams)
{
	// Release the texture view resource.
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

inline void DXFrameBuffer::enable()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

inline void DXFrameBuffer::disable()
{

}

inline void DXFrameBuffer::clear(float red, float green, float blue, float alpha)
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

inline void DXFrameBuffer::render()
{

}

inline void DXFrameBuffer::create(FactoryCreateParams * createParams)
{

}

inline void DXFrameBuffer::destroy(FactoryDestroyParams * destroyParams)
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

	return;
}

NAMESPACE_END