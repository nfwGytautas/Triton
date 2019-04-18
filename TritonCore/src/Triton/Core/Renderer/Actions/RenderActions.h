#pragma once
#include "Triton\Core\Renderer\RenderAction.h"
#include "Triton\Core\Data\Material.h"
#include "Triton\Core\Data\Mesh.h"
#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace RenderActions
	{
		class Prepare : public RenderAction
		{
		public:
			Prepare() { }

			// Inherited via RenderAction
			virtual void Execute() override;
		private:
		};

		class ChangeMaterial : public RenderAction
		{
		public:
			ChangeMaterial(std::shared_ptr<Data::Material> aMaterial);

			// Inherited via RenderAction
			virtual void Execute() override;
		private:
			std::shared_ptr<Data::Material> m_Material;
		};

		class ChangeMesh : public RenderAction
		{
		public:
			ChangeMesh(std::shared_ptr<Data::Mesh> aMesh);

			// Inherited via RenderAction
			virtual void Execute() override;
		private:
			std::shared_ptr<Data::Mesh> m_Mesh;
		};

		class BindShader : public RenderAction
		{
		public:
			BindShader(std::shared_ptr<Core::Shader> aShader);

			// Inherited via RenderAction
			virtual void Execute() override;
		private:
			std::shared_ptr<Core::Shader> m_Shader;
		};

		class ChangeShaderUniform : public RenderAction 
		{
		public:
			ChangeShaderUniform(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform);

			// Inherited via RenderAction
			virtual void Execute() override;
		private:
			std::shared_ptr<ShaderUniforms::ShaderUniform> m_Uniform;
			std::shared_ptr<Core::Shader> m_Shader;
		};

		class Render : public RenderAction
		{
		public:
			Render(unsigned int aIndiceCount);

			// Inherited via RenderAction
			virtual void Execute() override;
		private:
			unsigned int m_IndiceCount;
		};
	}
}