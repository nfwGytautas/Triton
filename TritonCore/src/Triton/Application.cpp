#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Events\EventManager.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

namespace Triton {

	Application::Application(const AppSettings& aSettings)
	{
#ifndef TR_DISABLE_GUI
		mGUIContext = aSettings.ImGUIContext;
		ImGui::SetCurrentContext(mGUIContext);
#endif

		prtc_Display = std::unique_ptr<Core::Display>(Core::Display::Create(Core::DisplaySettings()));

		prtc_Shader = std::unique_ptr<Core::Shader>(Core::Shader::Create(
			Core::ShaderSettings(
				"D:/Programming/Test files/nfw/shaders/triton/v4.shader", 
				"D:/Programming/Test files/nfw/shaders/triton/f4.shader")));

		prtc_Renderer = std::unique_ptr<Core::Renderer>(Core::Renderer::Create(prtc_Shader.get()));

		prtc_GUIS = std::make_unique<UI::GUICollection>();
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
		
		if (prtc_RenderOrder != nullptr)
		{
			prtc_Shader->Enable();
			prtc_Renderer->Render(*prtc_RenderOrder);
			prtc_Shader->Disable();
		}

#ifndef TR_DISABLE_GUI
		prtc_GUIS->RefreshCollection();
		prtc_GUIS->UpdateCollection(prtc_Delta);
		prtc_GUIS->VisualizeCollection();
		prtc_GUIS->DrawCollection();
#endif

		prtc_Display->OnUpdate();
	}

	void Application::UpdateProjectionMatrix()
	{
		prtc_Shader->Enable();
		glm::mat4 projection = Triton::Core::CreateProjectionMatrix(prtc_Display->GetWidth(), prtc_Display->GetHeight(), 45.0f, 0.1f, 100.0f);
		prtc_Shader->SetUniform("projectionMatrix", projection);
		glViewport(0, 0, prtc_Display->GetWidth(), prtc_Display->GetHeight());
		prtc_Shader->Disable();
	}

	void Application::Execute()
	{
		UpdateProjectionMatrix();

		PreExecutionSetup();
		while (!prtc_Display->Closed())
		{
			Run();
		}
	}
}