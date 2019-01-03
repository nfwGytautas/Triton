#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Structures\Mesh.h"

namespace Triton
{
	typedef unsigned int MeshID;
	typedef std::vector<std::shared_ptr<Data::Mesh>> MeshList;

	namespace Storage
	{
		//Class for storing loaded/created meshes
		class TRITON_API MeshStorage
		{
		public:
			MeshStorage();
			~MeshStorage();

			std::shared_ptr<Data::Mesh> Store(Data::Mesh* aMesh)
			{
				m_Meshes.push_back(std::shared_ptr<Data::Mesh>{ aMesh });
				return m_Meshes[m_Meshes.size() - 1];
			}
		private:
			MeshList m_Meshes;
		};

	}
}