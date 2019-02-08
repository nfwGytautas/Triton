#pragma once
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{

		struct TRITON_API TextureData
		{
			int Width;
			int Height;
			int BPP;
			std::unique_ptr<unsigned char> Buffer;

			TextureData()
				:Width(0), Height(0), BPP(0)
			{
				Buffer = std::make_unique<unsigned char>();
			}

			void Fill(std::string& aPathToImage);
		};

		class TRITON_API Texture
		{
		public:
			virtual ~Texture() { }

			unsigned int GetWidth() const
			{
				return m_Width;
			}
			unsigned int GetHeight() const
			{
				return m_Height;
			}

			static std::shared_ptr<Texture> Create(TextureData& aData);
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
		};

	}
}