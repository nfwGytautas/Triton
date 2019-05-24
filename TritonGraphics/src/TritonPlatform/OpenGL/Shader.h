#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN


unsigned int CreateProgram(std::initializer_list<unsigned int> aShaders)
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
unsigned int CreateShader(const char* type, std::string& source)
{
	char const* SourcePointer = source.c_str();
	unsigned int id = 0;

	int Result = 0;
	int InfoLogLength;

	if (type == "vertex")
	{
		id = glCreateShader(GL_VERTEX_SHADER);
		TR_CORE_TRACE("Compiling [VERTEX] shader");
	}
	else if (type == "fragment")
	{
		id = glCreateShader(GL_FRAGMENT_SHADER);
		TR_CORE_TRACE("Compiling [FRAGMENT] shader");
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


inline void Triton::PType::GLShader::setUniformInt(const std::string& aUniformName, const int& aParameter)
{
	glUniform1i(GetUniformLocation(aUniformName), aParameter);
}

inline void Triton::PType::GLShader::setUniformFloat(const std::string& aUniformName, const float& aParameter)
{
	glUniform1f(GetUniformLocation(aUniformName), aParameter);
}

inline void Triton::PType::GLShader::setUniformVector2(const std::string& aUniformName, const Vector2 aParameter)
{
	glUniform2f(GetUniformLocation(aUniformName), aParameter.x, aParameter.y);
}

inline void Triton::PType::GLShader::setUniformVector3(const std::string& aUniformName, const Vector3 aParameter)
{
	glUniform3f(GetUniformLocation(aUniformName), aParameter.x, aParameter.y, aParameter.z);
}

inline void Triton::PType::GLShader::setUniformVector4(const std::string& aUniformName, const Vector4 aParameter)
{
	glUniform4f(GetUniformLocation(aUniformName), aParameter.x, aParameter.y, aParameter.z, aParameter.w);
}

inline void Triton::PType::GLShader::setUniformMatrix44(const std::string& aUniformName, const Matrix44& aParameter)
{
	glUniformMatrix4fv(GetUniformLocation(aUniformName), 1, GL_FALSE, &aParameter[0][0]);
}

inline void Triton::PType::GLShader::enable()
{
	glUseProgram(m_shaderID);
	enabled = true;
}

inline void Triton::PType::GLShader::disable()
{
	glUseProgram(0);
	enabled = false;
}

inline void Triton::PType::GLShader::create(FactoryCreateParams* createParams)
{
	auto shaderCreate = OBJECT_AS(ShaderCreateParams, createParams);

	if (shaderCreate == nullptr)
	{
		TR_CORE_TRACE("Passed params for shader creation are not of type ShaderCreateParams");
		return;
	}

	TR_CORE_TRACE("Loading shader: \n	Vertex: {0}\n	Fragment: {1}", shaderCreate->vertexPath, shaderCreate->fragmentPath);

	//~READING~
	std::string fragmentSource;
	std::string vertexSource;

	std::ifstream vertexFile(shaderCreate->vertexPath);
	if (vertexFile.is_open()) {
		vertexSource.assign((std::istreambuf_iterator<char>(vertexFile)),
			(std::istreambuf_iterator<char>()));
	}
	else {
		TR_CORE_ERROR("Impossible to read {0}", shaderCreate->vertexPath);
		return;
	}
	vertexFile.close();
	// Read the Fragment Shader code from the file
	std::ifstream fragmentFile(shaderCreate->fragmentPath);
	if (fragmentFile.is_open()) {
		fragmentSource.assign((std::istreambuf_iterator<char>(fragmentFile)),
			(std::istreambuf_iterator<char>()));
	}
	else {
		TR_CORE_ERROR("Impossible to read {0}", shaderCreate->fragmentPath);
		return;
	}
	fragmentFile.close();
	//!READING!

	// Create the shaders
	unsigned int VertexShaderID = CreateShader("vertex", vertexSource);
	unsigned int FragmentShaderID = CreateShader("fragment", fragmentSource);

	m_shaderID = CreateProgram({ VertexShaderID , FragmentShaderID });

	TR_CORE_INFO("Loading shader complete. ID: {0}", m_shaderID);
}

inline void Triton::PType::GLShader::destroy(FactoryDestroyParams* destroyParams)
{
	glDeleteProgram(m_shaderID);
}

inline int Triton::PType::GLShader::GetUniformLocation(const std::string& aName)
{
	if (!enabled)
	{
		TR_CORE_WARN("Trying to get shader location of disabled shader!");
	}

	if (m_UniformLocationCache.find(aName) != m_UniformLocationCache.end())
		return m_UniformLocationCache[aName];

	int location = glGetUniformLocation(m_shaderID, aName.c_str());
	if (location == -1)
	{
		TR_CORE_WARN("Warning: uniform '{0}' doesn't exist!", aName);
	}

	m_UniformLocationCache[aName] = location;
	return location;
}


NAMESPACE_END