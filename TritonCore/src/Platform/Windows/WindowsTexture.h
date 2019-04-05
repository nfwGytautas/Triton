#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Texture.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API WindowsTexture : public Data::Texture
		{
		public:
			WindowsTexture(Triton::Data::TextureData& aData);
			~WindowsTexture();

			void Bind(unsigned int aSlot = 0);
			void Unbind(unsigned int aSlot = 0);
		private:
			unsigned int m_TextureID;
		};
	}
}