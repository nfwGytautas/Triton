#include "TRpch.h"
#include "Shader.h"

#include "Triton\Core\GraphicsAPI.h"
#include "Triton\Logger\Log.h"

Triton::Core::Shader::Shader(const char* vertexFilepath, const char* fragmentFilepath)
{
	TR_CORE_TRACE("Loading shader: \n	Vertex: {0}\n	Fragment: {1}", vertexFilepath, fragmentFilepath);

	//~READING~
	std::string fragmentSource;
	std::string vertexSource;

	std::ifstream vertexFile(vertexFilepath);
	if (vertexFile.is_open()) {
		vertexSource.assign((std::istreambuf_iterator<char>(vertexFile)),
			(std::istreambuf_iterator<char>()));
	}
	else {
		TR_CORE_ERROR("Impossible to read {0}", vertexFilepath);
		return;
	}
	vertexFile.close();
	// Read the Fragment Shader code from the file
	std::ifstream fragmentFile(fragmentFilepath);
	if (fragmentFile.is_open()) {
		fragmentSource.assign((std::istreambuf_iterator<char>(fragmentFile)),
			(std::istreambuf_iterator<char>()));
	}
	else {
		TR_CORE_ERROR("Impossible to read {0}", fragmentFilepath);
		return;
	}
	fragmentFile.close();
	//!READING!

	// Create the shaders
	unsigned int VertexShaderID = GraphicsAPI::CreateShader(GraphicsAPI::ShaderType::Vertex, vertexSource);
	unsigned int FragmentShaderID = GraphicsAPI::CreateShader(GraphicsAPI::ShaderType::Fragment, fragmentSource);

	m_programID = GraphicsAPI::CreateProgram({ VertexShaderID , FragmentShaderID });

	TR_CORE_INFO("Loading shader complete. ID: {0}", m_programID);
}

Triton::Core::Shader::~Shader()
{
}