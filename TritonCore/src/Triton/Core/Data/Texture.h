#pragma once
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{

		struct  TextureData
		{
			int Width;
			int Height;
			int BPP;
			std::unique_ptr<unsigned char> Buffer;
			std::string Path;

			TextureData()
				:Width(0), Height(0), BPP(0), Path("")
			{
				Buffer = std::make_unique<unsigned char>();
			}
			TextureData(std::string aPath)
				:Width(0), Height(0), BPP(0), Path(aPath)
			{
				Buffer = std::make_unique<unsigned char>();
			}
		};

		class  Texture
		{
		public:
			virtual ~Texture() { }

			virtual void Bind(unsigned int aSlot = 0) = 0;
			virtual void Unbind(unsigned int aSlot = 0) = 0;

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
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
			std::string m_Path;
		};

	}
}