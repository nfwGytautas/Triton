#pragma once

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN

inline void Triton::PType::DXTexture::enable()
{
	// Set shader texture resource in the pixel shader.
	m_deviceContext->PSSetShaderResources(0, 1, &m_textureView);
}

inline void Triton::PType::DXTexture::disable()
{
	
}

inline void Triton::PType::DXTexture::create(FactoryCreateParams* createParams)
{
	auto texParams = OBJECT_AS(TextureCreateParams, createParams);

	
}

inline void Triton::PType::DXTexture::destroy(FactoryDestroyParams* destroyParams)
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


NAMESPACE_END