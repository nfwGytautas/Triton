#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Core/Context.h"

namespace Triton
{
	namespace Graphics
	{
		class DXContext : public Context
		{
		private:
			/// Variable to track if the descriptions have been created already
			static bool s_descsCreated;

			static D3D11_BUFFER_DESC s_matrixBufferDesc;
			static D3D11_BUFFER_DESC s_settingsBufferDesc;
			static D3D11_BUFFER_DESC s_plightBufferDesc;
			static D3D11_BUFFER_DESC s_slightBufferDesc;
			static D3D11_BUFFER_DESC s_dlightBufferDesc;
			static D3D11_BUFFER_DESC s_camBufferDesc;
			static D3D11_SAMPLER_DESC s_samplerDesc;

			/**
			 * Creates the context description structures
			 */
			static void createDescs();
		public:
			/// Create the DX context
			DXContext();

			~DXContext();

			// Inherited via Context
			virtual Window* newWindow() override;
			virtual Renderer* newRenderer(Window* window) override;

			virtual bool init(EngineSettings& settings) override;
			virtual void shutdown() override;

			virtual Shader* newShader(const IO::ShaderData& createParams) override;
			virtual VAO* newVAO(const IO::Mesh& createParams) override;
			virtual Texture* newTexture(const IO::ImageData& createParams) override;
			virtual CubeTexture* newCubeTexture(const IO::ImageArrayData& createParams) override;
			virtual Framebuffer* newFramebuffer(unsigned int width, unsigned int height) override;
			virtual Bitmap* newBitmap(const BitmapCreateParams& createParams) override;
		private:
			ID3D11Device* m_device;
			ID3D11DeviceContext* m_deviceContext;

			std::vector<DXGI_MODE_DESC> m_displayModeList;

			std::string m_appNameSTD;
			LPCWSTR m_appName;

			int m_videoCardMemory;
			char m_videoCardDescription[128];
		};
	}
}