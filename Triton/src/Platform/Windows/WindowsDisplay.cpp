#include "TRpch.h"
#include "WindowsDisplay.h"

#include "Triton\Logger\Log.h"

#include "Triton\Events\EventManager.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

static bool s_GLFWInitialized = false;

Triton::Core::Display* Triton::Core::Display::Create(const DisplaySettings& aSettings)
{
	return new WindowsDisplay(aSettings);
}

Triton::Core::WindowsDisplay::WindowsDisplay(const DisplaySettings& aSettings)
{
	Init(aSettings);
}

Triton::Core::WindowsDisplay::~WindowsDisplay()
{
	ShutDown();
}

bool Triton::Core::WindowsDisplay::Closed()
{
	return glfwWindowShouldClose(m_Window);
}

void Triton::Core::WindowsDisplay::OnUpdate()
{
	//Check errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		TR_CORE_WARN("OpenGL error: {0}", error);
	}

	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

unsigned int Triton::Core::WindowsDisplay::GetWidth() const
{
	return m_Data.Width;
}

unsigned int Triton::Core::WindowsDisplay::GetHeight() const
{
	return m_Data.Height;
}

void Triton::Core::WindowsDisplay::SetVSync(bool aState)
{
	if (aState)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	m_Data.VSync = aState;
}

bool Triton::Core::WindowsDisplay::IsVSync()
{
	return m_Data.VSync;
}

void Triton::Core::WindowsDisplay::Init(const DisplaySettings& aSettings)
{
	m_Data.Title = aSettings.Title;
	m_Data.Width = aSettings.Width;
	m_Data.Height = aSettings.Height;

	TR_CORE_INFO("Creating a WINDOWS display: W:{0} H:{1} T:'{2}'", m_Data.Width, m_Data.Height, m_Data.Title);

	if (!s_GLFWInitialized)
	{
		int result = !glfwInit();
		if (result)
		{
			TR_CORE_ERROR("GLFW Initialization error");
			TR_CORE_ASSERT(result, "GLFW Initialization failed!");
		}
		else
		{
			int major, minor;
			glfwGetVersion(&major, &minor, nullptr);
			TR_CORE_INFO("GLFW version: {0}.{1} initialization successful", major, minor);
		}
	}

	m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);

	if (m_Window)
	{
		TR_CORE_INFO("Context creation successful");
	}
	else
	{
		TR_CORE_ERROR("Context creation failed!");
		TR_CORE_ASSERT(m_Window, "Context creation failed!");
	}

	//Makes the window context current
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);	
	SetUpCallbacks();
}

void Triton::Core::WindowsDisplay::ShutDown()
{
	glfwDestroyWindow(m_Window);
}

void Triton::Core::WindowsDisplay::SetUpCallbacks()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	auto errorCallback = [](int aCode, const char* aDescription)
	{
		TR_CORE_ERROR("GLFW Error: {0} '{1}'", aCode, aDescription);
	};

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

	auto mouseMoveCallback = [](GLFWwindow* w, double x, double y)
	{
		Triton::Core::EventManager::Post(new MouseMovedEvent(x, y));
	};

	auto windowResizeCallback = [](GLFWwindow* w, int width, int height)
	{
		Triton::Core::EventManager::Post(new WindowResizeEvent(width, height));
	};

	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(m_Window, keyCallback);
	glfwSetCursorPosCallback(m_Window, mouseMoveCallback);
	glfwSetWindowSizeCallback(m_Window, windowResizeCallback);
}