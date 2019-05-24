#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN

	// Creates the window
	inline void GLWindow::create(unsigned int width, unsigned height)
	{
		TR_CORE_INFO("Creating a WINDOWS display: W:{0} H:{1}", width, height);

		m_Window = glfwCreateWindow(width, height, "", NULL, NULL);

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
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TR_CORE_ASSERT(status, "glad initialization error");
	}

	// Get window size
	inline std::tuple<int, int> GLWindow::getWindowSize()
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);

		return { width, height };
	}

	// Check if the window is closed
	inline bool GLWindow::windowClosed()
	{
		return glfwWindowShouldClose(m_Window);
	}

	// Update the context
	inline void GLWindow::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	// Set window vsync property
	inline void GLWindow::setVsync(bool value)
	{
		glfwSwapInterval((int)value);
	}

	// Set cursor visability
	inline void GLWindow::showCursor(bool value)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, value ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	// Clear window contents
	inline void Triton::PType::GLWindow::clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Destroys the window
	inline void GLWindow::destroy()
	{
		glfwDestroyWindow(m_Window);
	}

NAMESPACE_END