#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Data structures\Model.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API WindowsMesh : public Mesh
		{
		public:
			WindowsMesh(std::vector<float>& aVertices, bool a3D);
			~WindowsMesh();

			// Inherited via Model
			void Render() override;
		private:
			std::vector<unsigned int> m_VAOS;
			std::vector<unsigned int> m_Indices;
			std::vector<unsigned int> m_VBOS;
		};

	}
}