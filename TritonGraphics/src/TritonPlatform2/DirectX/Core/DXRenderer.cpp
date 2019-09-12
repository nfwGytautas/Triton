#include "DXRenderer.h"

#include "TritonPlatform2/CrossTypes/Base.h"
#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	namespace Graphics
	{
		DXRenderer::DXRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<DXGI_MODE_DESC>& displayModeList, DXWindow* window)
			: m_deviceContext(deviceContext)
		{
			m_renderingTo = static_cast<Window*>(window);
			if (!init(device, displayModeList))
			{
				TR_SYSTEM_ERROR("Failed to initialize a renderer!");
			}
		}

		void DXRenderer::destroy()
		{
			// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
			if (m_swapChain)
			{
				m_swapChain->SetFullscreenState(false, NULL);
			}

			if (m_depthDisabledStencilState)
			{
				m_depthDisabledStencilState->Release();
				m_depthDisabledStencilState = 0;
			}

			if (m_rasterState)
			{
				m_rasterState->Release();
				m_rasterState = 0;
			}

			if (m_depthStencilView)
			{
				m_depthStencilView->Release();
				m_depthStencilView = 0;
			}

			if (m_depthStencilState)
			{
				m_depthStencilState->Release();
				m_depthStencilState = 0;
			}

			if (m_depthStencilBuffer)
			{
				m_depthStencilBuffer->Release();
				m_depthStencilBuffer = 0;
			}

			if (m_renderTargetView)
			{
				m_renderTargetView->Release();
				m_renderTargetView = 0;
			}

			if (m_swapChain)
			{
				m_swapChain->Release();
				m_swapChain = 0;
			}
		}

		void DXRenderer::newFrame(float red, float green, float blue, float alpha)
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

		void DXRenderer::endFrame()
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

			m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

			return;
		}

		void DXRenderer::default()
		{
			m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		}

		void DXRenderer::render(reference<Renderable>& renderable)
		{
			m_deviceContext->DrawIndexed(renderable->getIndiceCount(), 0, 0);
		}

		void DXRenderer::render(unsigned int indiceCount)
		{
			m_deviceContext->DrawIndexed(indiceCount, 0, 0);
		}

		void DXRenderer::setViewPort(int x, int y, int width, int height)
		{
			D3D11_VIEWPORT viewport;
			viewport.Width = (float)width;
			viewport.Height = (float)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;

			// Set the view port
			m_deviceContext->RSSetViewports(1, &viewport);
		}

		void DXRenderer::depthBufferState(bool state)
		{
			if (state)
			{
				m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
			}
			else
			{
				m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
			}
		}

		void DXRenderer::cullBufferState(bool state)
		{
			if (state)
			{
				m_deviceContext->RSSetState(m_rasterState);
			}
			else
			{
				m_deviceContext->RSSetState(m_disabledCullingState);
			}
		}

		bool DXRenderer::init(ID3D11Device* device, const std::vector<DXGI_MODE_DESC>& displayModeList)
		{
			HRESULT result;

			// First lets get the refresh rate for the monitor
			unsigned int numerator, denominator;

			auto[width, height] = m_renderingTo->size();
			for (int i = 0; i < displayModeList.size(); i++)
			{
				if (displayModeList[i].Width == (unsigned int)width)
				{
					if (displayModeList[i].Height == (unsigned int)height)
					{
						numerator = displayModeList[i].RefreshRate.Numerator;
						denominator = displayModeList[i].RefreshRate.Denominator;
					}
				}
			}

			DXGI_SWAP_CHAIN_DESC swapChainDesc;

			IDXGIDevice* pDXGIDevice = nullptr;
			result = device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get IDXGIDevice!");
				return false;
			}

			IDXGIAdapter* pDXGIAdapter = nullptr;
			result = pDXGIDevice->GetAdapter(&pDXGIAdapter);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get IDXGIAdapter!");
				return false;
			}

			IDXGIFactory* pIDXGIFactory = nullptr;
			result = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get IDXGIFactory!");
				return false;
			}

			pDXGIDevice->Release();
			pDXGIAdapter->Release();
			
			// Initialize the swap chain description.
			ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
			
			// Set to a single back buffer.
			swapChainDesc.BufferCount = 1;
			
			// Set the width and height of the back buffer.
			swapChainDesc.BufferDesc.Width = width;
			swapChainDesc.BufferDesc.Height = height;
			
			// Set regular 32-bit surface for the back buffer.
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			
			// Set the refresh rate of the back buffer.
			if (m_vsync)
			{
				swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
			}
			else
			{
				swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			}
			
			// Set the usage of the back buffer.
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			
			// Set the handle for the window to render to.
			swapChainDesc.OutputWindow = static_cast<DXWindow*>(m_renderingTo)->nativeHandle();
			
			// Turn multi sampling off.
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			
			// Set to full screen or windowed mode.
			if (m_renderingTo->isFullscreen())
			{
				swapChainDesc.Windowed = false;
			}
			else
			{
				swapChainDesc.Windowed = true;
			}
			
			// Set the scan line ordering and scaling to unspecified.
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			
			// Discard the back buffer contents after presenting.
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			
			// Don't set the advanced flags.
			swapChainDesc.Flags = 0;
			
			// Create the swap chain, Direct3D device, and Direct3D device context.
			result = pIDXGIFactory->CreateSwapChain(device, &swapChainDesc, &m_swapChain);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create a swap chain!");
				return false;
			}
			
			pIDXGIFactory->Release();

			// Lets get the render target view
			ID3D11Texture2D* backBufferPtr;

			// Get the pointer to the back buffer.
			result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get back buffer pointer from swap chain!");
				return false;
			}

			// Create the render target view with the back buffer pointer.
			result = device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get render target view from swap chain!");
				return false;
			}

			// Release pointer to the back buffer as we no longer need it.
			backBufferPtr->Release();
			backBufferPtr = 0;

			// Now get the depth buffer
			D3D11_TEXTURE2D_DESC depthBufferDesc;

			// Initialize the description of the depth buffer.
			ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

			// Set up the description of the depth buffer.
			depthBufferDesc.Width = width;
			depthBufferDesc.Height = height;
			depthBufferDesc.MipLevels = 1;
			depthBufferDesc.ArraySize = 1;
			depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthBufferDesc.SampleDesc.Count = 1;
			depthBufferDesc.SampleDesc.Quality = 0;
			depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthBufferDesc.CPUAccessFlags = 0;
			depthBufferDesc.MiscFlags = 0;

			// Create the texture for the depth buffer using the filled out description.
			result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to acquire depth stencil buffer");
				return false;
			}

			// Get the depth stencil
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

			// Initialize the description of the stencil state.
			ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

			// Set up the description of the stencil state.
			depthStencilDesc.DepthEnable = true;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			depthStencilDesc.StencilEnable = true;
			depthStencilDesc.StencilReadMask = 0xFF;
			depthStencilDesc.StencilWriteMask = 0xFF;

			// Stencil operations if pixel is front-facing.
			depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Stencil operations if pixel is back-facing.
			depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Create the depth stencil state.
			result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create depth stencil state");
				return false;
			}

			// Set the depth stencil state.
			m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

			// Initialize the depth stencil view.
			ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

			// Set up the depth stencil view description.
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			// Create the depth stencil view.
			result = device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create depth stencil view");
				return false;
			}

			// Bind the render target view and depth stencil buffer to the output render pipeline.
			m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

			// Set up the rasterizer
			D3D11_RASTERIZER_DESC rasterDesc;

			// Setup the raster description which will determine how and what polygons will be drawn.
			rasterDesc.AntialiasedLineEnable = false;
			rasterDesc.CullMode = D3D11_CULL_BACK;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.ScissorEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			// Create the rasterizer state from the description we just filled out.
			result = device->CreateRasterizerState(&rasterDesc, &m_rasterState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create rasterizer state");
				return false;
			}


			// Disabled culling state
			D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

			rasterDesc.CullMode = D3D11_CULL_NONE;

			// Create the disabled cull mode rasterizer state
			result = device->CreateRasterizerState(&rasterDesc, &m_disabledCullingState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create disabled culling rasterizer state");
				return false;
			}

			// Now set the rasterizer state.
			m_deviceContext->RSSetState(m_rasterState);

			// Setup the view port for rendering.
			D3D11_VIEWPORT viewport;

			viewport.Width = (float)width;
			viewport.Height = (float)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;

			// Create the view port.
			m_deviceContext->RSSetViewports(1, &viewport);

			// Clear the second depth stencil state before setting the parameters.
			ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

			// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
			// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
			depthDisabledStencilDesc.DepthEnable = false;
			depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			depthDisabledStencilDesc.StencilEnable = true;
			depthDisabledStencilDesc.StencilReadMask = 0xFF;
			depthDisabledStencilDesc.StencilWriteMask = 0xFF;
			depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Create the state using the device.
			result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create disabled depth stencil");
				return false;
			}

			// Create the projection matrix for 3D rendering.
			recalcMatrices();
			m_worldMatrix = DirectX::XMMatrixIdentity();

			return true;
		}
	}
}