#pragma once
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{
		struct TRITON_API MeshData
		{
			std::vector<float> Vertices;
			std::vector<float> UVs;

			void operator<<(std::string& aPathToMesh);
		};

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

			static std::shared_ptr<Mesh> Create(MeshData& aData, bool a3D = true);
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
		};

	}
}