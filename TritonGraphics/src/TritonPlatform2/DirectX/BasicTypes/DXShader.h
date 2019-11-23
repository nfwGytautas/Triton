#pragma once

#include "TritonPlatform2/DirectX/DXMacros.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

namespace Triton
{
	namespace Graphics
	{

		class DXContext;

		class DXShader : public Shader
		{
			const int c_SettingsBufferSlot = 0;
			const int c_MatrixBufferSlot = 1;

			const int c_PointLightsSlot = 2;
			const int c_SpotLightsSlot = 3;
			const int c_DirectionalLightsSlot = 4;

			const int c_CameraSlot = 5;
		public:
			DXShader(const Flags::ShaderFlagset& flags);
			virtual ~DXShader();

			// Inherited via Shader
			virtual void enable() override;
			virtual void disable() override;
			virtual void update_matrices() override;
		private:
			void mapBuffer(ID3D11Buffer* buffer, D3D11_MAPPED_SUBRESOURCE& mappedResource);
			void unmapBuffer(ID3D11Buffer* buffer);

			void updateSettingsBuffer();
			void updateMatricesBuffer();
			void updateLightingBuffer();

			void updatePLightBuffer();
			void updateSLightBuffer();
			void updateDLightBuffer();

			void updateCameraBuffer();
		private:
			ID3D11DeviceContext* m_deviceContext = nullptr;
			ID3D11VertexShader* m_vertexShader = nullptr;
			ID3D11PixelShader* m_pixelShader = nullptr;
			ID3D11InputLayout* m_layout = nullptr;
			ID3D11SamplerState* m_sampleState = nullptr;

			ID3D11Buffer* m_matrixBuffer = nullptr;
			ID3D11Buffer* m_settingsBuffer = nullptr;

			ID3D11Buffer* m_plightBuffer = nullptr;
			ID3D11Buffer* m_slightBuffer = nullptr;
			ID3D11Buffer* m_dlightBuffer = nullptr;

			ID3D11Buffer* m_cameraBuffer = nullptr;

			friend DXContext;
		};

	}
}