#pragma once

#include "TritonPlatform\CrossTypes\Types.h"
#include "Triton/AppSettings.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// General types expanded on in platform specific way.

namespace Triton
{
	namespace PType
	{
		class DXContext;
		class DXFactory;
		class DXRenderer;

		class DXTexture : public Texture
		{
		public:
			virtual ~DXTexture() { }

			virtual void enable() override;
			virtual void disable() override;
		protected:
			// Inherited via VAO
			virtual void create(FactoryCreateParams* createParams) override;
			virtual void destroy(FactoryDestroyParams* destroyParams) override;
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_textureView;

			friend DXRenderer;
			friend DXFactory;
		};

		class DXFrameBuffer : public FrameBuffer
		{
		public:
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
			virtual void destroy(FactoryDestroyParams* destroyParams) override;
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11ShaderResourceView* m_shaderResourceView;
			ID3D11Texture2D* m_renderTargetTexture;
			ID3D11DepthStencilView* m_depthStencilView;

			friend DXRenderer;
			friend DXFactory;
		};

		class DXVAO : public VAO
		{
		private:
			struct VertexType
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT2 texture;
				DirectX::XMFLOAT3 normal;
			};
		public:
			virtual ~DXVAO() { }

			virtual void enable() override;
			virtual void disable() override;
		protected:
			// Inherited via VAO
			virtual void create(FactoryCreateParams* createParams) override;
			virtual void destroy(FactoryDestroyParams* destroyParams) override;
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11Buffer *m_vertexBuffer;
			ID3D11Buffer *m_indexBuffer;
			unsigned int m_indiceCount;

			friend DXRenderer;
			friend DXFactory;
		};

		class DXShader : public Shader
		{
		public:
			DXShader(ShaderLayout* layout);
			virtual ~DXShader() { }

			// Inherited via Shader
			virtual void enable() override;
			virtual void disable() override;

		protected:
			// Inherited via Shader
			virtual void create(FactoryCreateParams* createParams) override;
			virtual void destroy(FactoryDestroyParams* destroyParams) override;

		protected:
			DXShader() 
			{ }

		private:
			void updateBuffer(Shader::Buffer& buffer);

			ID3D11Buffer* getBuffer(const std::string& name);
			void mapBuffer(ID3D11Buffer* buffer, D3D11_MAPPED_SUBRESOURCE& mappedResource);
			void unmapBuffer(ID3D11Buffer* buffer);
			void setBuffer(ID3D11Buffer* buffer, const BufferShaderType& type, unsigned int count, unsigned int number);
		private:
			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader* m_pixelShader;
			ID3D11InputLayout* m_layout;
			ID3D11DeviceContext* m_deviceContext;
			ID3D11SamplerState* m_sampleState;

			std::unordered_map<std::string, ID3D11Buffer*> m_buffers;

			friend DXFactory;

			// Inherited via Shader
			virtual void updateBuffers(BufferUpdateType type) override;
		};

		class DXFactory : public Factory
		{
		public:
			FactoryObject* createShader(FactoryCreateParams* createParams) override;
			FactoryObject* createVAO(FactoryCreateParams* createParams) override;
			FactoryObject* createTexture(FactoryCreateParams * createParams) override;
			FactoryObject* createFramebuffer(FactoryCreateParams * createParams) override;

			void destroyObject(FactoryObject* object, FactoryDestroyParams* destroyParams) override;

		private:
			void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
		private:
			HWND m_hwnd;
			ID3D11Device* m_device;
			ID3D11DeviceContext* m_deviceContext;
			ID3D11DepthStencilView* m_depthStencilView;

			friend DXContext;
		};

		class DXWindow : public Window
		{
		public:
			// Inherited via Window
			virtual void create(unsigned int width, unsigned height) override;
			virtual std::tuple<int, int> getWindowSize() override;
			virtual bool windowClosed() override;
			virtual void update() override;
			virtual void setVsync(bool value) override;
			virtual void showCursor(bool value) override;
			virtual void clear(float r, float g, float b, float a) override;
			virtual void destroy() override;

			LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
		private:
			Core::EventReceiver* m_receiver;
			bool m_fullscreen = false;

			bool m_hasWindow;

			LPCWSTR m_applicationName;
			HINSTANCE m_hinstance;
			HWND m_hwnd;
			IDXGISwapChain* m_swapChain;

			// Current message from windows
			MSG m_msg;

			bool m_vsync;

			friend DXContext;
		};

		class DXRenderer : public Renderer
		{
		public:
			// Inherited via Renderer
			virtual void create() override;
			virtual void render(PlatformObject* renderable) override;
			virtual void default() override;

			virtual void newFrame(float red, float green, float blue, float alpha) override;
			virtual void endFrame() override;

			virtual Matrix44 projection() override;
			virtual Matrix44 recalc_projection() override;
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11DepthStencilView* m_depthStencilView;

			DirectX::XMMATRIX m_projectionMatrix;
			DirectX::XMMATRIX m_worldMatrix;
			DirectX::XMMATRIX m_orthoMatrix;

			friend DXContext;

			

			// Inherited via Renderer

		};

		class DXContext : public Context
		{
			inline Triton::PType::DXWindow* dx_window()
			{
				return static_cast<Triton::PType::DXWindow*>(window);
			}

			inline Triton::PType::DXRenderer* dx_renderer()
			{
				return static_cast<Triton::PType::DXRenderer*>(renderer);
			}

			inline Triton::PType::DXFactory* dx_factory()
			{
				return static_cast<Triton::PType::DXFactory*>(factory);
			}
		public:
			Core::EventReceiver* receiver;
		public:
			DXContext(const Triton::AppSettings& appSettings);
			virtual ~DXContext() { }

			// Inherited via Context
			virtual bool init() override;
			virtual bool init_additional() override;
			virtual void shutdown() override;
			virtual void setContextEventCallBacks(Core::EventReceiver* receiver) override;
			virtual void update() override;
			virtual void setViewPort(int x, int y, int width, int height) override;
			virtual void fillPacket(PlatformObject* packet) override;
		private:
			std::string m_appNameSTD;
			LPCWSTR m_appName;

			int m_videoCardMemory;
			char m_videoCardDescription[128];
			IDXGISwapChain* m_swapChain;
			ID3D11Device* m_device;
			ID3D11DeviceContext* m_deviceContext;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11Texture2D* m_depthStencilBuffer;
			ID3D11DepthStencilState* m_depthStencilState;
			ID3D11DepthStencilView* m_depthStencilView;
			ID3D11RasterizerState* m_rasterState;
		};
	}
}