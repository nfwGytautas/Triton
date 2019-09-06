#pragma once

#include "TritonPlatform/DirectX/Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXTexture : public Texture
{
public:
	virtual ~DXTexture();

	virtual void enable() override;
	virtual void disable() override;

	virtual void* raw_texture() override
	{
		return m_textureView;
	}
protected:
	virtual void create(FactoryCreateParams* createParams) override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

	friend DXRenderer;
	friend DXFactory;
};

class DXCubeTexture : public CubeTexture
{
public:
	virtual ~DXCubeTexture();

	virtual void enable() override;
	virtual void disable() override;

	virtual void* raw_texture() override
	{
		return m_textureView;
	}
protected:
	virtual void create(FactoryCreateParams* createParams) override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

	friend DXRenderer;
	friend DXFactory;
};

PLATFORM_NAMESPACE_END