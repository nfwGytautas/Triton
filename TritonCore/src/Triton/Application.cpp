#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Events\EventManager.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

namespace Triton {

	Application::Application()
	{
		prtc_Display = std::unique_ptr<Core::Display>(Core::Display::Create(Core::DisplaySettings()));

		prtc_Shader = std::unique_ptr<Core::Shader>(Core::Shader::Create(
			Core::ShaderSettings(
				"D:/Programming/Test files/nfw/shaders/triton/v3.shader", 
				"D:/Programming/Test files/nfw/shaders/triton/f3.shader")));

		prtc_Renderer = std::unique_ptr<Core::Renderer>(Core::Renderer::Create(prtc_Shader.get()));

		glViewport(0, 0, prtc_Display->GetWidth(), prtc_Display->GetHeight());
	}
	
	Application::~Application()
	{

	}

	void Application::Run()
	{	
		float currentFrame = glfwGetTime();
		prtc_Delta = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;

		OnUpdate();	

		Core::EventManager::Dispatch();
		
		if (prtc_RenderBatch != nullptr)
		{
			prtc_Shader->Enable();
			prtc_Renderer->Render(*prtc_RenderBatch);
			prtc_Shader->Disable();
		}

		prtc_Display->OnUpdate();
	}

	void Application::Execute()
	{
		PreExecutionSetup();
		while (!prtc_Display->Closed())
		{
			Run();
		}
	}
}