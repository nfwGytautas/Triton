#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Events\EventManager.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"
#include "Core\GraphicsAPI.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Triton {

	Application::Application()
	{
		prtc_Display = std::unique_ptr<Core::Display>(Core::Display::Create(Core::DisplaySettings()));
		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());
		Core::GraphicsAPI::ChangeViewport(prtc_Display->GetWidth(), prtc_Display->GetHeight());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		prtc_Shader = CreateShader();

		unsigned int vao;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		float vertices_position[] = {
				0.0, 0.0,
				0.5, 0.0,
				0.5, 0.5,

				0.0, 0.0,
				0.0, 0.5,
				-0.5, 0.5,

				0.0, 0.0,
				-0.5, 0.0,
				-0.5, -0.5,

				0.0, 0.0,
				0.0, -0.5,
				0.5, -0.5,
		};

		unsigned int vbo;
		glGenBuffers(1, &vbo);

		// Allocate space and upload the data from CPU to GPU
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

		// Get the location of the attributes that enters in the vertex shader
		GLint position_attribute = prtc_Shader->GetAttribLocation("position");

		// Specify how the data for position can be accessed
		glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Enable the attribute
		glEnableVertexAttribArray(position_attribute);

		while (!prtc_Display->Closed())
		{			
			glClear(GL_COLOR_BUFFER_BIT);
			
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 12);
		

			prtc_Display->OnUpdate();
			Core::EventManager::Dispatch();
			prtc_EntityRegistry->UpdateSystems();
		}
	}
}