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
			std::string Path;

			TextureData()
				:Width(0), Height(0), BPP(0), Path("null")
			{
				Buffer = std::make_unique<unsigned char>();
			}

			void Fill(const char* aPathToImage);
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
			std::string GetPath() const
			{
				return m_Path;
			}

			static std::shared_ptr<Texture> Create(TextureData& aData);
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
			std::string m_Path;
		};

	}
}