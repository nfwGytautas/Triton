#pragma once
#include "Triton\TRMacros.h"

#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Data
	{
		struct MeshData
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

		class Mesh
		{
		public:
			virtual ~Mesh() { }

			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			unsigned int GetIndiceCount() const
			{
				return m_IndiceCount;
			}

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
			unsigned int m_IndiceCount;
		};

	}
}