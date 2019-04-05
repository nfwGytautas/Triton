#pragma once
#include "Triton\TRMacros.h"

#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Data
	{
		struct TRITON_API MeshData
		{
			struct Vertex
			{
				Vector3 Vertice;
				Vector2 UV;
				Vector3 Normal;
			};

			std::vector<Vertex> Vertices;

			std::vector<unsigned int> Indices;

			std::string Path;

			MeshData() {}
			MeshData(std::string aPath) : Path(aPath) {}
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
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
		};

	}
}