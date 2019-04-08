#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Texture.h"

namespace Triton
{
	namespace Core
	{
		class  WindowsTexture : public Data::Texture
		{
		public:
			WindowsTexture(Triton::Data::TextureData& aData);
			~WindowsTexture();

			void Bind(unsigned int aSlot = 0) override;
			void Unbind(unsigned int aSlot = 0) override;
		private:
			unsigned int m_TextureID;
		};
	}
}