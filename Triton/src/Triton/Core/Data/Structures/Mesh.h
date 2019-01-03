#pragma once
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Storage
	{
		class MeshStorage;
	}

	namespace Data
	{

		class TRITON_API Mesh
		{
		public:
			virtual ~Mesh() { }

			unsigned int GetWidth() const
			{
				return m_Width;
			}
			unsigned int GetHeight() const
			{
				return m_Height;
			}

			static std::shared_ptr<Mesh> Create(Storage::MeshStorage& aStorage, std::vector<float>& aVertices, std::vector<float>& aColorData, bool a3D = true);
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
		};

	}
}