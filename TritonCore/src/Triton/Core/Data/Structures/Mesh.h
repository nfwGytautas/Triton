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

			//std::vector<Vertex> Vertices;

			std::vector<float> Vertices;
			std::vector<float> UVS;
			std::vector<float> Normals;


			std::vector<unsigned int> Indices;

			void Fill(const char* aPathToMesh);
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

			static std::shared_ptr<Mesh> Create(MeshData& aData);
		protected:
			unsigned int m_Width;
			unsigned int m_Height;
		};

	}
}