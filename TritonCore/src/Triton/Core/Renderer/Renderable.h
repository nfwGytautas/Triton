#pragma once
#include "Triton\TRMacros.h"

#include "Triton\Core\Shader\Shader.h"
#include "Triton\Core\Data\Mesh.h"
#include "Triton\Core\Data\Material.h"

namespace Triton
{
	class TRITON_API MaterialEntry
	{
	public:
		MaterialEntry(std::shared_ptr<Data::Material> aMaterial, std::string aShaderName, int aShaderLocation)
			: Material(aMaterial), ShaderName(aShaderName), ShaderLocation(aShaderLocation)
		{ }

		std::shared_ptr<Data::Material> Material;
		std::string ShaderName;
		int ShaderLocation;
	};

	namespace Core
	{
		class TRITON_API Renderable
		{
		public:
			Renderable(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<Data::Mesh> aMesh);
			virtual ~Renderable() { }

			void AddMaterial(MaterialEntry aMatEntry);
			void AddUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform);

			void AddRenderUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform);

			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void Render() = 0;

		private:
			virtual void Prepare() = 0;
		protected:
			std::shared_ptr<Core::Shader> m_Shader;
			std::shared_ptr<Data::Mesh> m_Mesh;
			std::vector<std::shared_ptr<ShaderUniforms::ShaderUniform>> m_Uniforms;
			std::vector<std::shared_ptr<ShaderUniforms::ShaderUniform>> m_RenderUniforms;
			std::vector<MaterialEntry> m_Materials;
		};
	}
}