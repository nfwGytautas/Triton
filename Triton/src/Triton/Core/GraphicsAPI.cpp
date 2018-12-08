#include "GraphicsAPI.h"

#include "Triton\Logger\Log.h"

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

void Triton::Core::GraphicsAPI::ChangeViewport(double Width, double Height)
{
	glViewport(0, 0, (GLsizei) Width, (GLsizei) Height);
}

TRITON_API TRITON_WINDOW_CONTEXT Triton::Core::GraphicsAPI::ContextManager::GetContext()
{
	return Context;
}

TRITON_API void Triton::Core::GraphicsAPI::ContextManager::SetContext(TRITON_WINDOW_CONTEXT aContext)
{
	Context = aContext;
}