#pragma once

#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXFramebuffer : public Framebuffer
{
public:
	virtual ~DXFramebuffer();

	// Inherited via FrameBuffer
	virtual void enable() override;
	virtual void disable() override;

	virtual void clear(float red, float green, float blue, float alpha) override;
	virtual void render() override;

	ID3D11Texture2D* getTextureData()
	{
		return m_renderTargetTexture;
	}
	ID3D11ShaderResourceView* getShaderResourceView()
	{
		return m_shaderResourceView;
	}
protected:
	virtual void create(FactoryCreateParams* createParams) override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11DepthStencilView* m_depthStencilView;

	friend DXRenderer;
	friend DXFactory;
};

PLATFORM_NAMESPACE_END