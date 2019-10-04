#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Texture.h"

namespace Triton
{
	namespace Graphics
	{

		class DXRenderer;
		class DXContext;

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
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_textureView;

			friend DXRenderer;
			friend DXContext;
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
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_textureView;

			friend DXRenderer;
			friend DXContext;
		};

	}
}