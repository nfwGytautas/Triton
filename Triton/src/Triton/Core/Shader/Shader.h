#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"
#include "Triton\Core\Math\Math.h"
#include "Triton\File\File.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API Shader
		{
		public:
			Shader(const char* vertexFilepath, const char* fragmentFilepath);
			~Shader();

		public:
			void setBooleanUniform(const std::string& name, bool v0);
			void setIntUniform(const std::string& name, int v0);
			void setFloatUniform(const std::string& name, float v0);
			void setVec2Uniform(const std::string& name, const Math::Vector3f& vec0);
			void setVec3Uniform(const std::string& name, const Math::Vector3f& vec0);
			void setMatrix4fUniform(const std::string& name, const Math::Vector3f& m0);
		private:
			std::unordered_map<std::string, int> m_UniformLocationCache;
		private:
			unsigned int m_programID;
			int getUniformLocation(const std::string& name);
		};
	}
}