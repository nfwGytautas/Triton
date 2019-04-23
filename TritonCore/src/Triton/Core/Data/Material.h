#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Math\Math.h"
#include "Triton\Core\Data\Texture.h"
#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace Data
	{
		class Material
		{
		public:
			Material(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<Texture> aTexture);

			virtual ~Material()
			{ }

			void Bind();
			void Unbind();

			std::shared_ptr<Core::Shader> Shader() const
			{
				return m_Shader;
			}
		private:
			std::shared_ptr<Core::Shader> m_Shader;
			std::shared_ptr<Texture> m_Texture;

			Vector3 m_Ambient;
			Vector3 m_Diffuse;
			Vector3 m_Specular;
			float m_Shininess;
		};

	}
}