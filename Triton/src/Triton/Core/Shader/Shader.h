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

			unsigned int GetAttribLocation(const std::string& aAttribName);
		public:
		private:
			std::unordered_map<std::string, int> m_UniformLocationCache;
		private:
			unsigned int m_programID;
			int getUniformLocation(const std::string& name);
		};
	}
}