#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN
	
	static bool s_GLFWInitialized;

	// Initializes GLFW and glad.
	bool GLContext::init()
	{
		if (!s_GLFWInitialized)
		{
			int result = !glfwInit();
			if (result)
			{
				TR_CORE_ERROR("GLFW Initialization error");
				TR_CORE_ASSERT(result, "GLFW Initialization failed!");
				return false;
			}
			else
			{
				int major, minor;
				glfwGetVersion(&major, &minor, nullptr);
				TR_CORE_INFO("GLFW version: {0}.{1} initialization successful", major, minor);
			}
		}

		window = new GLWindow();
		renderer = new GLRenderer();
		factory = new GLFactory();

		return true;
	}

	// Destroys context and any resources that are used by it.
	void GLContext::shutdown()
	{
		window->destroy();
		delete window;
		delete renderer;
		delete factory;
	}

	// Sets the event receiver for the context
	void GLContext::setContextEventCallBacks(Core::EventReceiver* receiver)
	{
		this->receiver = receiver;
		auto* window = static_cast<Triton::PType::GLWindow*>(this->window)->m_Window;
		glfwSetWindowUserPointer(window, this);

		auto errorCallback = [](int aCode, const char* aDescription)
		{
			TR_CORE_ERROR("GLFW Error: {0} '{1}'", aCode, aDescription);
		};
		glfwSetErrorCallback(errorCallback);


		auto keyCallback = [](GLFWwindow* w, int key, int scancode, int action, int mods)
		{
			GL_CONTEXT_FROM_GLFW_WINDOW(w);

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

				context.receiver->OnKeyPressed(key, repeat, scancode, mods);
				prevKey = key;
				break;
			case GLFW_RELEASE:
				context.receiver->OnKeyReleased(key, scancode, mods);
				break;
			}
		};
		glfwSetKeyCallback(window, keyCallback);


		auto mouseMoveCallback = [](GLFWwindow* w, double x, double y)
		{
			GL_CONTEXT_FROM_GLFW_WINDOW(w);

			context.receiver->OnMouseMoved(x, y);
		};
		glfwSetCursorPosCallback(window, mouseMoveCallback);


		auto windowResizeCallback = [](GLFWwindow* w, int width, int height)
		{
			GL_CONTEXT_FROM_GLFW_WINDOW(w);

			glViewport(0, 0, width, height);
			context.receiver->OnWindowResized(width, height);
		};
		glfwSetWindowSizeCallback(window, windowResizeCallback);


		auto mouseKeyCallback = [](GLFWwindow* w, int button, int action, int mods)
		{
			GL_CONTEXT_FROM_GLFW_WINDOW(w);

			if (action == GLFW_PRESS)
			{
				context.receiver->OnMouseButtonPressed(button);
			}
			else if (action == GLFW_RELEASE)
			{
				context.receiver->OnMouseButtonReleased(button);
			}
		};
		glfwSetMouseButtonCallback(window, mouseKeyCallback);


		auto scrollCallback = [](GLFWwindow* w, double xoffset, double yoffset)
		{
			GL_CONTEXT_FROM_GLFW_WINDOW(w);

			context.receiver->OnMouseScrolled(xoffset, yoffset);
		};
		glfwSetScrollCallback(window, scrollCallback);


		auto charCallback = [](GLFWwindow* w, unsigned int charInput)
		{
			GL_CONTEXT_FROM_GLFW_WINDOW(w);

			context.receiver->OnKeyInput(charInput);
		};
		glfwSetCharCallback(window, charCallback);
	}

	void GLContext::update()
	{
		window->update();
	}

	void Triton::PType::GLContext::setViewPort(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

NAMESPACE_END