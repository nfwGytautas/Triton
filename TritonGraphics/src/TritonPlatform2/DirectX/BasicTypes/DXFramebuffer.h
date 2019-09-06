#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Texture.h"

PLATFORM_NAMESPACE_BEGIN

class DXRenderer;
class DXContext;

class DXFramebuffer : public Framebuffer
{
public:
	virtual ~DXFramebuffer();

	// Inherited via FrameBuffer
	virtual void enable() override;
	virtual void disable() override;

	virtual void clear(float red, float green, float blue, float alpha) override;
	virtual void render() override;

	virtual void* getAsTexture() override
	{
		return m_shaderResourceView;
	}
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11DepthStencilView* m_depthStencilView;

	friend DXRenderer;
	friend DXContext;
};

PLATFORM_NAMESPACE_END