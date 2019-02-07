#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Structures\Texture.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API WindowsTexture : public Data::Texture
		{
		public:
			WindowsTexture(unsigned int aWidth, unsigned int aHeight, unsigned char* aBuffer);
			~WindowsTexture();

			void Bind(unsigned int aSlot = 0);
			void Unbind();
		private:
			unsigned int m_TextureID;
		};
	}
}