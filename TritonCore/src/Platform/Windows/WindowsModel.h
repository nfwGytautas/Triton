#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Mesh.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API WindowsMesh : public Data::Mesh
		{
		public:
			WindowsMesh(Data::MeshData& aData);
			~WindowsMesh();

			void Bind();
			void Unbind();
			unsigned int GetIndiceCount() const;

		private:
			unsigned int m_VAO;
			unsigned int m_IBO;
			std::vector<unsigned int> m_VBOS;
			unsigned int m_IndiceCount;
		};

	}
}