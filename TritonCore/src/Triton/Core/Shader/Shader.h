#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"
#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Core
	{
		struct ShaderSettings
		{
			std::string VertexFilepath;
			std::string FragmentFilepath;

			ShaderSettings(std::string aVertexPath, std::string aFragmentPath)
				:VertexFilepath(aVertexPath), FragmentFilepath(aFragmentPath)
			{ }
		};

		class TRITON_API Shader
		{
		public:
			virtual ~Shader() { }

			virtual void SetUniform(const std::string& aUniformName, const      int& aParameter) = 0;
			virtual void SetUniform(const std::string& aUniformName, const    float& aParameter) = 0;
			virtual void SetUniform(const std::string& aUniformName, const  Vector2& aParameter) = 0;
			virtual void SetUniform(const std::string& aUniformName, const  Vector3& aParameter) = 0;
			virtual void SetUniform(const std::string& aUniformName, const  Vector4& aParameter) = 0;
			virtual void SetUniform(const std::string& aUniformName, const Matrix44& aParameter) = 0;		

			virtual void Enable() = 0;
			virtual void Disable() = 0;

			static Shader* Create(const ShaderSettings& settings);
		};
	}

	namespace ShaderUniforms
	{
		class TRITON_API ShaderUniform
		{
		public:
			virtual void Set(Core::Shader& shader) = 0;

			ShaderUniform(std::string aName)
				: m_Name(aName) {}
		protected:
			std::string m_Name;
		};

		TR_SHADER_UNIFORM_DEFINITION(int, )
		TR_SHADER_UNIFORM_DEFINITION(float, )
		TR_SHADER_UNIFORM_DEFINITION(Vector2, Triton::)
		TR_SHADER_UNIFORM_DEFINITION(Vector3, Triton::)
		TR_SHADER_UNIFORM_DEFINITION(Vector4, Triton::)
		TR_SHADER_UNIFORM_DEFINITION(Matrix44, Triton::)
	}
}