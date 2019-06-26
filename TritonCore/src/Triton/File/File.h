#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

#include "TritonPlatform\PlatformAbstraction.h"

namespace Triton
{
	namespace Data
	{
		
		class  File
		{
		public:
			static std::string ReadAllText(std::string& aPath);
			static bool Valid(std::string& aPath);

			static void ReadMesh(std::string aPath, Triton::PType::VAOCreateParams* params);
			static void ReadTexture(std::string aPath, Triton::PType::TextureCreateParams* params);

			// Reads and creates a shader buffer layout from specified shaders in the create params
			// NOTE: The create params must already contain the path to both pixel and vertex shaders
			static PType::ShaderLayout ReadShaderLayout(Triton::PType::ShaderCreateParams* params);

			// Checks if the specified string is a shader type
			static PType::ShaderDataType isShaderVariable(const std::string& str);

		private:
			// Read the path and parse it into buffers and struct
			// Returns bool that shows if the operation was successful
			static bool readShaderStream(
				std::string& path, 
				PType::BufferShaderType shaderType,
				std::string& entryPoint,
				std::vector<PType::ShaderBufferLayout>& buffers, 
				std::vector<PType::ShaderInputLayout>& structs,
				std::string& inputStructName);
		private:
			static std::unordered_map<std::string, PType::ShaderDataType> m_typeMap;
		};

	}
}