#include "DXTexture.h"

PLATFORM_NAMESPACE_BEGIN

DXTexture::~DXTexture()
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

void DXTexture::enable()
{
	// Set shader texture resource in the pixel shader.
	m_deviceContext->PSSetShaderResources(0, 1, &m_textureView);
}

void DXTexture::disable()
{

}

void DXTexture::create(FactoryCreateParams* createParams)
{
	auto texParams = OBJECT_AS(TextureCreateParams, createParams);


}

PLATFORM_NAMESPACE_END