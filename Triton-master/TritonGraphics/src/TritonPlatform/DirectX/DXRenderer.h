#pragma once

#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXRenderer : public Renderer
{
public:
	// Inherited via Renderer
	virtual void create() override;
	virtual void render(reference<Renderable>& renderable) override;
	virtual void render(unsigned int indiceCount) override;
	virtual void default() override;

	virtual void newFrame(float red, float green, float blue, float alpha) override;
	virtual void endFrame() override;

	virtual Matrix44 projection() override;
	virtual Matrix44 orthographic() override;
	virtual void recalc_matrices() override;

	virtual void setVsync(bool value) override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
	IDXGISwapChain* m_swapChain;

	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_orthographicMatrix;
	DirectX::XMMATRIX m_worldMatrix;

	friend DXContext;
};

PLATFORM_NAMESPACE_END