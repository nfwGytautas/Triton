#include "DXRenderer.h"

#include "TritonPlatform2/CrossTypes/Base.h"
#include "TritonPlatform2/mathematical.h"

#include "Triton2/Utility/Log.h"

#include "Triton2/Assert.h"

namespace Triton
{
	namespace Graphics
	{
		bool DXRenderer::s_descsCreated = false;
		D3D11_DEPTH_STENCIL_DESC DXRenderer::s_defaultDepthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC DXRenderer::s_defaultDepthStencilViewDesc;
		D3D11_RASTERIZER_DESC DXRenderer::s_defaultRasterDesc;
		D3D11_RASTERIZER_DESC DXRenderer::s_noCullRasterDesc;
		D3D11_DEPTH_STENCIL_DESC DXRenderer::s_disabledDepthStencilDesc;
		D3D11_BLEND_DESC DXRenderer::s_blendEnableDesc;
		D3D11_BLEND_DESC DXRenderer::s_blendDisableDesc;

		void DXRenderer::createDescs()
		{
			TR_SYSTEM_TRACE("Creating default depth stencil");
			// Initialize the description of the stencil state.
			ZeroMemory(&s_defaultDepthStencilDesc, sizeof(s_defaultDepthStencilDesc));

			// Set up the description of the stencil state.
			s_defaultDepthStencilDesc.DepthEnable = true;
			s_defaultDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			s_defaultDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			s_defaultDepthStencilDesc.StencilEnable = true;
			s_defaultDepthStencilDesc.StencilReadMask = 0xFF;
			s_defaultDepthStencilDesc.StencilWriteMask = 0xFF;

			// Stencil operations if pixel is front-facing.
			s_defaultDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			s_defaultDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			s_defaultDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			s_defaultDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Stencil operations if pixel is back-facing.
			s_defaultDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			s_defaultDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			s_defaultDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			s_defaultDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			TR_SYSTEM_TRACE("Creating default depth stencil view");

			// Initialize the depth stencil view.
			ZeroMemory(&s_defaultDepthStencilViewDesc, sizeof(s_defaultDepthStencilViewDesc));

			// Set up the depth stencil view description.
			s_defaultDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			s_defaultDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			s_defaultDepthStencilViewDesc.Texture2D.MipSlice = 0;

			TR_SYSTEM_TRACE("Creating default raster description");
			// Setup the raster description which will determine how and what polygons will be drawn.
			s_defaultRasterDesc.AntialiasedLineEnable = false;
			s_defaultRasterDesc.CullMode = D3D11_CULL_BACK;
			s_defaultRasterDesc.DepthBias = 0;
			s_defaultRasterDesc.DepthBiasClamp = 0.0f;
			s_defaultRasterDesc.DepthClipEnable = true;
			s_defaultRasterDesc.FillMode = D3D11_FILL_SOLID;
			s_defaultRasterDesc.FrontCounterClockwise = false;
			s_defaultRasterDesc.MultisampleEnable = false;
			s_defaultRasterDesc.ScissorEnable = false;
			s_defaultRasterDesc.SlopeScaledDepthBias = 0.0f;

			TR_SYSTEM_TRACE("Creating no culling raster description");
			s_noCullRasterDesc = s_defaultRasterDesc;
			s_noCullRasterDesc.CullMode = D3D11_CULL_NONE;


			TR_SYSTEM_TRACE("Creating disabled depth stencil");

			// Clear the second depth stencil state before setting the parameters.
			ZeroMemory(&s_disabledDepthStencilDesc, sizeof(s_disabledDepthStencilDesc));

			// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
			// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
			s_disabledDepthStencilDesc.DepthEnable = false;
			s_disabledDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			s_disabledDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			s_disabledDepthStencilDesc.StencilEnable = true;
			s_disabledDepthStencilDesc.StencilReadMask = 0xFF;
			s_disabledDepthStencilDesc.StencilWriteMask = 0xFF;
			s_disabledDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			s_disabledDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			s_disabledDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			s_disabledDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			s_disabledDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			s_disabledDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			s_disabledDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			s_disabledDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			TR_SYSTEM_TRACE("Creating blend state descriptions");

			// Clear the blend state description.
			ZeroMemory(&s_blendEnableDesc, sizeof(D3D11_BLEND_DESC));

			s_blendEnableDesc.RenderTarget[0].BlendEnable = TRUE;
			s_blendEnableDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			s_blendEnableDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			s_blendEnableDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			s_blendEnableDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			s_blendEnableDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			s_blendEnableDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			s_blendEnableDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

			s_blendDisableDesc = s_blendEnableDesc;
			s_blendDisableDesc.RenderTarget[0].BlendEnable = FALSE;

			s_descsCreated = true;
		}

		DXGI_SWAP_CHAIN_DESC DXRenderer::createDescs2(const std::vector<DXGI_MODE_DESC>& displayModeList)
		{
			auto[width, height] = m_renderingTo->size();

			// First lets get the refresh rate for the monitor
			unsigned int numerator, denominator;

			TR_SYSTEM_TRACE("Getting display refresh rate");
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

			TR_SYSTEM_INFO("Refresh rate: '{0}' '{1}'", numerator, denominator);

			TR_SYSTEM_TRACE("Creating swap chain description");

			DXGI_SWAP_CHAIN_DESC swapChainDesc;

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
				TR_SYSTEM_TRACE("V_SYNC enabled");
				swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
			}
			else
			{
				TR_SYSTEM_TRACE("V_SYNC disabled");
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
				TR_SYSTEM_TRACE("Full screen enabled");
				swapChainDesc.Windowed = false;
			}
			else
			{
				TR_SYSTEM_TRACE("Full screen disabled");
				swapChainDesc.Windowed = true;
			}

			// Set the scan line ordering and scaling to unspecified.
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			// Discard the back buffer contents after presenting.
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			// Don't set the advanced flags.
			swapChainDesc.Flags = 0;
			
			return swapChainDesc;
		}

		std::tuple<D3D11_TEXTURE2D_DESC, D3D11_VIEWPORT> DXRenderer::createDescs3()
		{
			auto[width, height] = m_renderingTo->size();

			TR_SYSTEM_TRACE("Creating depth buffer description");

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

			TR_SYSTEM_TRACE("Creating view port");
			// Setup the view port for rendering.
			D3D11_VIEWPORT viewport;

			viewport.Width = (float)width;
			viewport.Height = (float)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;

			return std::tuple<D3D11_TEXTURE2D_DESC, D3D11_VIEWPORT>(depthBufferDesc, viewport);
		}

		DXRenderer::DXRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<DXGI_MODE_DESC>& displayModeList, DXWindow* window)
			: m_deviceContext(deviceContext)
		{
			TR_SYSTEM_TRACE("Checking renderer state descriptions");
			if (!s_descsCreated)
			{
				TR_SYSTEM_WARN("Renderer descriptions not created, creating now");
				createDescs();
			}
			else
			{
				TR_SYSTEM_TRACE("Descriptions already created");
			}

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

			freeBuffers();

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

		void DXRenderer::alphaBlending(bool state)
		{
			float blendFactor[4];

			// Setup the blend factor.
			blendFactor[0] = 0.0f;
			blendFactor[1] = 0.0f;
			blendFactor[2] = 0.0f;
			blendFactor[3] = 0.0f;

			// Turn off the alpha blending.
			m_deviceContext->OMSetBlendState(state ? m_alphaEnableBlendingState : m_alphaDisableBlendingState, blendFactor, 0xffffffff);

			return;
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

		void DXRenderer::resize()
		{
			TR_SYSTEM_TRACE("Resizing the renderer swap chain");

			m_deviceContext->OMSetRenderTargets(0, 0, 0);

			freeBuffers();

			HRESULT hr;

			TR_SYSTEM_TRACE("Resizing buffers");
			hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			if (FAILED(hr))
			{
				TR_SYSTEM_ERROR("Failed to resize buffers");
				return;
			}

			auto[width, height] = m_renderingTo->size();
			setViewPort(0, 0, width, height);

			recalcMatrices();

			createBuffers();
		}

		std::tuple<int, int> DXRenderer::size()
		{
			return m_renderingTo->size();
		}

		bool DXRenderer::init(ID3D11Device* device, const std::vector<DXGI_MODE_DESC>& displayModeList)
		{
			TR_SYSTEM_INFO("Initializing renderer");

			HRESULT result;

			auto swapChainDesc = createDescs2(displayModeList);
			auto[depthBufferDesc, viewport] = createDescs3();

			m_device = device;
			
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

			TR_SYSTEM_TRACE("Creating swap chain instance");
			// Create the swap chain, Direct3D device, and Direct3D device context.
			result = pIDXGIFactory->CreateSwapChain(device, &swapChainDesc, &m_swapChain);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create a swap chain!");
				return false;
			}
			
			TR_SYSTEM_INFO("Swap chain created");

			pIDXGIFactory->Release();

			if (!createBuffers())
			{
				TR_SYSTEM_ERROR("Failed to create render buffers");
				return false;
			}

			m_initialized = true;

			TR_SYSTEM_INFO("Renderer initialized");

			return true;
		}
		bool DXRenderer::createBuffers()
		{
			// Free previous buffer instances
			freeBuffers();

			auto [depthBufferDesc, viewport] = createDescs3();

			// Lets get the render target view
			ID3D11Texture2D* backBufferPtr;

			TR_SYSTEM_TRACE("Creating renderer states");

			HRESULT result;

			// Get the pointer to the back buffer.
			result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get back buffer pointer from swap chain!");
				return false;
			}

			// Create the render target view with the back buffer pointer.
			result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get render target view from swap chain!");
				return false;
			}

			// Release pointer to the back buffer as we no longer need it.
			backBufferPtr->Release();
			backBufferPtr = 0;

			// Create the texture for the depth buffer using the filled out description.
			result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to acquire depth stencil buffer");
				return false;
			}


			// Create the depth stencil state.
			result = m_device->CreateDepthStencilState(&s_defaultDepthStencilDesc, &m_depthStencilState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create depth stencil state");
				return false;
			}

			// Set the depth stencil state.
			m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

			// Create the depth stencil view.
			result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &s_defaultDepthStencilViewDesc, &m_depthStencilView);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create depth stencil view");
				return false;
			}

			// Bind the render target view and depth stencil buffer to the output render pipeline.
			m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

			// Create the rasterizer state from the description we just filled out.
			result = m_device->CreateRasterizerState(&s_defaultRasterDesc, &m_rasterState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create rasterizer state");
				return false;
			}

			// Create the disabled cull mode rasterizer state
			result = m_device->CreateRasterizerState(&s_noCullRasterDesc, &m_disabledCullingState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create disabled culling rasterizer state");
				return false;
			}

			// Now set the rasterizer state.
			m_deviceContext->RSSetState(m_rasterState);

			// Create the view port.
			m_deviceContext->RSSetViewports(1, &viewport);

			// Create the state using the device.
			result = m_device->CreateDepthStencilState(&s_disabledDepthStencilDesc, &m_depthDisabledStencilState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create disabled depth stencil");
				return false;
			}

			TR_SYSTEM_INFO("Creating blend states");

			result = m_device->CreateBlendState(&s_blendEnableDesc, &m_alphaEnableBlendingState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create alpha blending enabled state");
				return false;
			}

			result = m_device->CreateBlendState(&s_blendDisableDesc, &m_alphaDisableBlendingState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create alpha blending disabled state");
				return false;
			}

			TR_SYSTEM_TRACE("Calculating matrices");

			// Create the projection matrix for 3D rendering.
			recalcMatrices();
			m_worldMatrix = DirectX::XMMatrixIdentity();

			return true;
		}

		void DXRenderer::freeBuffers()
		{
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

			if (m_disabledCullingState)
			{
				m_disabledCullingState->Release();
				m_disabledCullingState = 0;
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

			if (m_alphaEnableBlendingState)
			{
				m_alphaEnableBlendingState->Release();
				m_alphaEnableBlendingState = 0;
			}

			if (m_alphaDisableBlendingState)
			{
				m_alphaDisableBlendingState->Release();
				m_alphaDisableBlendingState = 0;
			}
		}
	}
}