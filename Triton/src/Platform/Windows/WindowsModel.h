#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Structures\Mesh.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API WindowsMesh : public Data::Mesh
		{
		public:
			WindowsMesh(std::vector<float>& aVertices, std::vector<float>& aUVs, bool a3D);
			~WindowsMesh();

			void Bind();
			void Unbind();
			unsigned int GetIndiceCount() const;
		private:
			unsigned int m_VAO;
			std::vector<unsigned int> m_VBOS;
			unsigned int m_IndiceCount;
		};

	}
}