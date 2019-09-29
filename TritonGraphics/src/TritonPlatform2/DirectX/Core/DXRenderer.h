#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Core/Renderer.h"

#include "TritonPlatform2/DirectX/Core/DXWindow.h"

PLATFORM_NAMESPACE_BEGIN

class DXRenderer : public Renderer
{
private:
	/// Variable to track if the descriptions have been created already
	static bool s_descsCreated;

	static D3D11_DEPTH_STENCIL_DESC s_defaultDepthStencilDesc;
	static D3D11_DEPTH_STENCIL_VIEW_DESC s_defaultDepthStencilViewDesc;
	static D3D11_RASTERIZER_DESC s_defaultRasterDesc;
	static D3D11_RASTERIZER_DESC s_noCullRasterDesc;
	static D3D11_DEPTH_STENCIL_DESC s_disabledDepthStencilDesc;
	
	/**
	 * Creates the renderer description structures
	 */
	static void createDescs();

	/**
	 * Creates the display specific descriptions
	 *
	 * @param displayModeList Possible default list got from the context
	 * @return A tuple of swap chain description, depth buffer description, view port
	 */
	std::tuple<DXGI_SWAP_CHAIN_DESC, D3D11_TEXTURE2D_DESC, D3D11_VIEWPORT> createDescs2(const std::vector<DXGI_MODE_DESC>& displayModeList);
public:
	DXRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<DXGI_MODE_DESC>& displayModeList, DXWindow* window);
	
	// Inherited via Renderer
	virtual void destroy() override;

	virtual void newFrame(float red, float green, float blue, float alpha) override;
	virtual void endFrame() override;

	virtual void default() override;

	virtual void render(reference<Renderable>& renderable) override;
	virtual void render(unsigned int indiceCount) override;

	virtual void setViewPort(int x, int y, int width, int height) override;
	virtual void depthBufferState(bool state) override;
	virtual void cullBufferState(bool state) override;
private:
	bool init(ID3D11Device* device, const std::vector<DXGI_MODE_DESC>& displayModeList);	
private:
	ID3D11DeviceContext* m_deviceContext;

	IDXGISwapChain* m_swapChain;
	ID3D11Texture2D* m_depthStencilBuffer;

	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_disabledCullingState;

	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
};

PLATFORM_NAMESPACE_END