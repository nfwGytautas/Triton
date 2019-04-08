#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Mesh.h"

namespace Triton
{
	namespace Core
	{

		class  WindowsMesh : public Data::Mesh
		{
		public:
			WindowsMesh(Data::MeshData& aData);
			~WindowsMesh();

			void Bind() override;
			void Unbind() override;

		private:
			unsigned int m_VAO;
			unsigned int m_IBO;
			std::vector<unsigned int> m_VBOS;
		};

	}
}