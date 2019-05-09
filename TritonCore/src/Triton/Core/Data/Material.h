#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Math\Math.h"
#include "Triton\Core\Data\Texture.h"
#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace Data
	{
		struct MaterialData
		{
			size_t Shader;
			size_t Texture;

			Vector3 Ambient;
			Vector3 Diffuse;
			Vector3 Specular;
			float Shininess;

			MaterialData()
				: Shader(-1), Texture(-1) {}
			MaterialData(size_t aShader, size_t aTexture)
				: Shader(aShader), Texture(aTexture) {}
		};

		class Material
		{
		public:
			Material(relay_ptr<Core::Shader> aShader, relay_ptr<Texture> aTexture);

			virtual ~Material()
			{ }

			virtual void Bind();
			virtual void Unbind();

			relay_ptr<Core::Shader> Shader()
			{
				return m_Shader;
			}
		private:
			relay_ptr<Core::Shader> m_Shader;
			relay_ptr<Texture> m_Texture;

			Vector3 m_Ambient;
			Vector3 m_Diffuse;
			Vector3 m_Specular;
			float m_Shininess;
		};

	}
}