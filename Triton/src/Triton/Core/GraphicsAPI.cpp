#include "TRpch.h"
#include "GraphicsAPI.h"

#include <GL\glew.h>

#include "Triton\Logger\Log.h"

#include "Triton\Events\EventManager.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

bool Triton::Core::GraphicsAPI::Initialize()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		TR_CORE_ERROR("GLEW Initialization error '{0}'", glewGetErrorString(err));
		return false;
	}
	else
	{
		TR_CORE_INFO("GLEW version: {0} initialization successful", glewGetString(GLEW_VERSION));
	}

	return true;
}

void Triton::Core::GraphicsAPI::ChangeViewport(double aWidth, double aHeight)
{
	glViewport(0, 0, (GLsizei) aWidth, (GLsizei) aHeight);
}

unsigned int Triton::Core::GraphicsAPI::CreateShader(ShaderType aShaderType, std::string& aShaderSource)
{
	char const* SourcePointer = aShaderSource.c_str();
	unsigned int id = 0;

	int Result = 0;
	int InfoLogLength;
	
	switch (aShaderType)
	{
	case ShaderType::Vertex :
		id = glCreateShader(GL_VERTEX_SHADER);
		TR_CORE_TRACE("Compiling [VERTEX] shader");
		break;
	case ShaderType::Fragment :
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

unsigned int Triton::Core::GraphicsAPI::CreateProgram(std::initializer_list<unsigned int> aShaders)
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

unsigned int Triton::Core::GraphicsAPI::GetShaderAttributeLocation(const unsigned int& aProgramID, const std::string& aAttributeName)
{
	return glGetAttribLocation(aProgramID, aAttributeName.c_str());
}