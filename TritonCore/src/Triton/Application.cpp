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

		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());

		prtc_Renderer = std::unique_ptr<Core::Renderer>(Core::Renderer::Create(prtc_Shader.get()));

		prtc_AppState = std::make_unique<Storage::AppState>();

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

		prtc_Shader->Enable();
		Matrix44 ProjMatrix = Core::CreateProjectionMatrix(prtc_Display->GetWidth(), prtc_Display->GetHeight(), 70, 2000.0f, 0.1f);
		prtc_Shader->SetUniform("projectionMatrix", ProjMatrix);		

		PreExecutionSetup();

		while (!prtc_Display->Closed())
		{
			float currentFrame = glfwGetTime();
			prtc_Delta = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;

			OnUpdate();	

			prtc_Camera->OnUpdate();
			prtc_Shader->SetUniform("viewMatrix", prtc_Camera->ViewMatrix());

			Core::EventManager::Dispatch();

			prtc_Renderer->Render(prtc_v_RenderBatch);
			prtc_Display->OnUpdate();
		}

		prtc_AppState->Clear();
	}

	void Application::Restart()
	{
		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());
		PreExecutionSetup();
	}
}