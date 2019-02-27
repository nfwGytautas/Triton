#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Math\Math.h"
#include "Triton\Core\Data\Structures\Texture.h"

namespace Triton
{
	namespace Data
	{

		class TRITON_API Material
		{
		public:
			Material(std::shared_ptr<Texture> aTexture)
				:m_Texture(aTexture), m_Diffuse(0.0f, 0.0f, 0.0f)
			{ }

			Material()
				:m_Texture(), m_Diffuse(0.0f, 0.0f, 0.0f)
			{ }

			~Material()
			{ }

			void SetDiffuse(Vector3 aValue)
			{
				m_Diffuse = aValue;
			}

			Vector3 GetDiffuse()
			{
				return m_Diffuse;
			}

			std::shared_ptr<Texture> GetTexture()
			{
				return m_Texture;
			}
		private:
			std::shared_ptr<Texture> m_Texture;
			Vector3 m_Diffuse;
		};

	}
}