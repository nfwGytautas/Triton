#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace Core
	{
		class  WindowsShader : public Shader
		{
		public:
			WindowsShader(const ShaderSettings& aSettings);
			virtual ~WindowsShader();
			
			virtual void SetUniform(const std::string& aUniformName, const      int& aParameter) override;
			virtual void SetUniform(const std::string& aUniformName, const    float& aParameter) override;
			virtual void SetUniform(const std::string& aUniformName, const  Vector2& aParameter) override;
			virtual void SetUniform(const std::string& aUniformName, const  Vector3& aParameter) override;
			virtual void SetUniform(const std::string& aUniformName, const  Vector4& aParameter) override;
			virtual void SetUniform(const std::string& aUniformName, const Matrix44& aParameter) override;

			virtual void Enable() override;
			virtual void Disable() override;
		private:
			enum class ShaderType
			{
				Vertex,
				Fragment,
			};

			unsigned int CreateShader(ShaderType aShaderType, std::string& aShaderSource);
			unsigned int CreateProgram(std::initializer_list<unsigned int> aShaders);

			int GetUniformLocation(const std::string& aName);
		private:
			std::unordered_map<std::string, int> m_UniformLocationCache;
			unsigned int m_programID;			
		};
	}
}