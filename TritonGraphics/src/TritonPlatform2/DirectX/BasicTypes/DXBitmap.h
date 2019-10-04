#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Bitmap.h"

namespace Triton
{
	namespace Graphics
	{

		class DXContext;

		class DXBitmap : public Bitmap
		{
		public:
			DXBitmap();
			virtual ~DXBitmap();

			virtual void enable() override;
			virtual void disable() override;

			virtual void setPosition(unsigned int newX, unsigned int newY) override;
			virtual unsigned int getIndiceCount() override;
		private:
			ID3D11DeviceContext* m_deviceContext;
			ID3D11Buffer* m_vertexBuffer;
			ID3D11Buffer* m_indexBuffer;
			unsigned int m_vertexCount;
			unsigned int m_indiceCount;

			friend DXContext;
		};

	}
}