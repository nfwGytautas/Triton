#include "TRpch.h"
#include "WindowsShader.h"

#include <glad\glad.h>
#include "Triton\Logger\Log.h"

Triton::Core::Shader* Triton::Core::Shader::Create(const ShaderSettings& aSettings)
{
	return new WindowsShader(aSettings);
}

Triton::Core::WindowsShader::WindowsShader(const ShaderSettings& aSettings)
{
	TR_CORE_TRACE("Loading shader: \n	Vertex: {0}\n	Fragment: {1}", aSettings.VertexFilepath, aSettings.FragmentFilepath);

	//~READING~
	std::string fragmentSource;
	std::string vertexSource;

	std::ifstream vertexFile(aSettings.VertexFilepath);
	if (vertexFile.is_open()) {
		vertexSource.assign((std::istreambuf_iterator<char>(vertexFile)),
			(std::istreambuf_iterator<char>()));
	}
	else {
		TR_CORE_ERROR("Impossible to read {0}", aSettings.VertexFilepath);
		return;
	}
	vertexFile.close();
	// Read the Fragment Shader code from the file
	std::ifstream fragmentFile(aSettings.FragmentFilepath);
	if (fragmentFile.is_open()) {
		fragmentSource.assign((std::istreambuf_iterator<char>(fragmentFile)),
			(std::istreambuf_iterator<char>()));
	}
	else {
		TR_CORE_ERROR("Impossible to read {0}", aSettings.FragmentFilepath);
		return;
	}
	fragmentFile.close();
	//!READING!

	// Create the shaders
	unsigned int VertexShaderID = CreateShader(ShaderType::Vertex, vertexSource);
	unsigned int FragmentShaderID = CreateShader(ShaderType::Fragment, fragmentSource);

	m_programID = CreateProgram({ VertexShaderID , FragmentShaderID });

	TR_CORE_INFO("Loading shader complete. ID: {0}", m_programID);
}

Triton::Core::WindowsShader::~WindowsShader()
{
	glDeleteProgram(m_programID);
}

int Triton::Core::WindowsShader::GetUniformLocation(const std::string& aName)
{
	if (m_UniformLocationCache.find(aName) != m_UniformLocationCache.end())
		return m_UniformLocationCache[aName];

	int location = glGetUniformLocation(m_programID, aName.c_str());
	if (location == -1)
	{
		TR_CORE_WARN("Warning: uniform '{0}' doesn't exist!", aName);
	}

	m_UniformLocationCache[aName] = location;
	return location;
}

void Triton::Core::WindowsShader::Enable()
{
	glUseProgram(m_programID);
}

void Triton::Core::WindowsShader::Disable()
{
	glUseProgram(0);
}

void Triton::Core::WindowsShader::SetUniform(const std::string & aUniformName, const int & aParameter)
{
	glUniform1i(GetUniformLocation(aUniformName), aParameter);
}

void Triton::Core::WindowsShader::SetUniform(const std::string & aUniformName, const float & aParameter)
{
	glUniform1f(GetUniformLocation(aUniformName), aParameter);
}

void Triton::Core::WindowsShader::SetUniform(const std::string & aUniformName, const Vector2 & aParameter)
{
	glUniform2f(GetUniformLocation(aUniformName), aParameter.x, aParameter.y);
}

void Triton::Core::WindowsShader::SetUniform(const std::string& aUniformName, const Vector3& aParameter)
{
	glUniform3f(GetUniformLocation(aUniformName), aParameter.x, aParameter.y, aParameter.z);
}

void Triton::Core::WindowsShader::SetUniform(const std::string& aUniformName, const Vector4& aParameter)
{
	glUniform4f(GetUniformLocation(aUniformName), aParameter.x, aParameter.y, aParameter.z, aParameter.w);
}

void Triton::Core::WindowsShader::SetUniform(const std::string& aUniformName, const Matrix44& aParameter)
{
	glUniformMatrix4fv(GetUniformLocation(aUniformName), 1, GL_FALSE, &aParameter[0][0]);
}

unsigned int Triton::Core::WindowsShader::CreateShader(ShaderType aShaderType, std::string& aShaderSource)
{
	char const* SourcePointer = aShaderSource.c_str();
	unsigned int id = 0;

	int Result = 0;
	int InfoLogLength;

	switch (aShaderType)
	{
	case ShaderType::Vertex:
		id = glCreateShader(GL_VERTEX_SHADER);
		TR_CORE_TRACE("Compiling [VERTEX] shader");
		break;
	case ShaderType::Fragment:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		TR_CORE_TRACE("Compiling [FRAGMENT] shader");
		break;
	}

	glShaderSource(id, 1, &SourcePointer, NULL);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		TR_CORE_ERROR("Shader compilation failed: {0}", &ShaderErrorMessage[0]);
	}

	return id;
}
unsigned int Triton::Core::WindowsShader::CreateProgram(std::initializer_list<unsigned int> aShaders)
{
	TR_CORE_TRACE("Linking program");

	unsigned int id = glCreateProgram();

	int Result = 0;
	int InfoLogLength;

	for (unsigned int shader : aShaders)
	{
		glAttachShader(id, shader);
	}

	glLinkProgram(id);
	glValidateProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &Result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		TR_CORE_ERROR("Shader PROGRAM linking failed: {0}", &ProgramErrorMessage[0]);
	}

	for (unsigned int shader : aShaders)
	{
		glDetachShader(id, shader);
		glDeleteShader(shader);
	}

	return id;
}