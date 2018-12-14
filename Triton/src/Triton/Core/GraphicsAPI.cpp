#include "TRpch.h"
#include "GraphicsAPI.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Triton\Logger\Log.h"

#include "Triton\Event manager\EventManager.h"
#include "Triton\Event manager\KeyEvent.h"
#include "Triton\Event manager\MouseEvent.h"
#include "Triton\Event manager\ApplicationEvent.h"

TRITON_API TRITON_WINDOW_CONTEXT Triton::Core::GraphicsAPI::ContextManager::Context = nullptr;

bool Triton::Core::GraphicsAPI::Initialize()
{
	TR_CORE_INFO("Triton engine");

	if (!glfwInit())
	{
		TR_CORE_ERROR("GLFW Initialization error");
		return false;
	}
	else
	{
		int major, minor;
		glfwGetVersion(&major, &minor, nullptr);
		TR_CORE_INFO("GLFW version: {0}.{1} initialization successful", major, minor);

		if (CreateContext())
		{
			TR_CORE_INFO("Context creation successful");
		}
		else
		{
			TR_CORE_ERROR("Context creation failed!");
			return false;
		}
	}

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

bool Triton::Core::GraphicsAPI::CreateContext()
{
	TRITON_WINDOW_CONTEXT Context = glfwCreateWindow(640, 640, "Window", NULL, NULL);

	//Checks if a window is created successfully
	if (!Context)
	{
		return false;
	}

	//Makes the window context current
	glfwMakeContextCurrent(Context);

	ChangeViewport(640, 640);

	ContextManager::SetContext(Context);

	return true;
}

TRITON_API void Triton::Core::GraphicsAPI::ContextLink(void* aLink)
{
	glfwSetWindowUserPointer(ContextManager::GetContext(), aLink);
}

void Triton::Core::GraphicsAPI::ChangeViewport(double aWidth, double aHeight)
{
	glViewport(0, 0, (GLsizei) aWidth, (GLsizei) aHeight);
}

TRITON_API void Triton::Core::GraphicsAPI::PollCallbacks()
{
	glfwPollEvents();
}

TRITON_API void Triton::Core::GraphicsAPI::SetupContextCallbacks()
{
	auto keyCallback = [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		static int prevKey = 0;
		static int repeat = 0;

		switch (action)
		{
			case GLFW_PRESS:
				repeat = -1;
			case GLFW_REPEAT:
				if (prevKey == key)
				{
					repeat++;
				}
				else
				{
					repeat = 0;
				}
				Triton::Core::EventManager::Post(new KeyPressedEvent(key, repeat, scancode, mods));	
				prevKey = key;
				break;
			case GLFW_RELEASE:
				Triton::Core::EventManager::Post(new KeyReleasedEvent(key, scancode, mods));
				break;
		}
	};

	glfwSetKeyCallback(ContextManager::GetContext(), keyCallback);
}

TRITON_API unsigned int Triton::Core::GraphicsAPI::CreateShader(ShaderType aShaderType, std::string& aShaderSource)
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

TRITON_API unsigned int Triton::Core::GraphicsAPI::CreateProgram(std::initializer_list<unsigned int> aShaders)
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

TRITON_API TRITON_WINDOW_CONTEXT Triton::Core::GraphicsAPI::ContextManager::GetContext()
{
	return Context;
}

TRITON_API void Triton::Core::GraphicsAPI::ContextManager::SetContext(TRITON_WINDOW_CONTEXT aContext)
{
	Context = aContext;
}