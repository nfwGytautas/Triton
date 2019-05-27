#pragma once

#include "TritonPlatform\CrossTypes\Types.h"

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

		class DXVAO : public VAO
		{
		private:
			struct VertexType
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT2 texture;
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
		private:
			struct MatrixBufferType
			{
				DirectX::XMMATRIX worldMatrix;
				DirectX::XMMATRIX viewMatrix;
				DirectX::XMMATRIX projectionMatrix;
			};

		public:
			virtual ~DXShader() { }

			// Inherited via Shader
			virtual void setUniformInt(const std::string & aUniformName, const int & aParameter) override;
			virtual void setUniformFloat(const std::string & aUniformName, const float & aParameter) override;
			virtual void setUniformVector2(const std::string & aUniformName, const Vector2 aParameter) override;
			virtual void setUniformVector3(const std::string & aUniformName, const Vector3 aParameter) override;
			virtual void setUniformVector4(const std::string & aUniformName, const Vector4 aParameter) override;
			virtual void setUniformMatrix44(const std::string & aUniformName, const Matrix44& aParameter) override;
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
			void updateCBufferMatrix(MatrixBufferType* buffer, const std::string& name, const Matrix44& matrix);
		private:
			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader* m_pixelShader;
			ID3D11InputLayout* m_layout;
			ID3D11Buffer* m_matrixBuffer;
			ID3D11DeviceContext* m_deviceContext;
			ID3D11SamplerState* m_sampleState;
			MatrixBufferType m_prevVal;

			friend DXFactory;
		};

		class DXFactory : public Factory
		{
		public:
			FactoryObject* createShader(FactoryCreateParams* createParams) override;
			FactoryObject* createVAO(FactoryCreateParams* createParams) override;
			FactoryObject* createTexture(FactoryCreateParams * createParams) override;

			void destroyObject(FactoryObject* object, FactoryDestroyParams* destroyParams) override;

		private:
			void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
		private:
			HWND m_hwnd;
			ID3D11Device* m_device;
			ID3D11DeviceContext* m_deviceContext;

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
			DXContext(std::string appName);
			virtual ~DXContext() { }

			// Inherited via Context
			virtual bool init() override;
			virtual bool init_additional() override;
			virtual void shutdown() override;
			virtual void setContextEventCallBacks(Core::EventReceiver* receiver) override;
			virtual void update() override;
			virtual void setViewPort(int x, int y, int width, int height) override;

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

#include "Init.h"

#include "Window.h"

#include "Renderer.h"

#include "Shader.h"

#include "Factory.h"

#include "VAO.h"

#include "Texture.h"