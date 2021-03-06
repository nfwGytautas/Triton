#pragma once

#include "TritonPlatform2/DirectX/DXMacros.h"
#include "TritonPlatform2/CrossTypes/VAO.h"

namespace Triton
{
	namespace Graphics
	{

		class DXRenderer;
		class DXContext;

		class DXVAO : public VAO
		{
		public:
			virtual ~DXVAO();

			// Inherited via VAO
			virtual void enable() override;
			virtual void disable() override;

			virtual unsigned int getIndiceCount() override;

			virtual void updateVertices(std::vector<float>& vertices) override;
			virtual void updateIndices(std::vector<unsigned int>& indices) override;
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11Buffer *m_vertexBuffer;
			ID3D11Buffer *m_indexBuffer;
			unsigned int m_indiceCount;

			friend DXRenderer;
			friend DXContext;
		};

	}
}