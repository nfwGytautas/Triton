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
		: prtc_DefaultRoutineValues()
	{
		prtc_Display = std::unique_ptr<Core::Display>(Core::Display::Create(
			Core::DisplaySettings(aSettings.WindowTitle, aSettings.WindowWidth, aSettings.WindowHeight)
		));
		prtc_Display->SetEventReceiver(this);

		prtc_Renderer = std::unique_ptr<Core::Renderer>(std::move(Platform::Create()));

		m_DefaultRenderRoutine = std::make_shared<Core::RenderRoutine>();
		prtc_DefaultRoutineValues.TransformationUniform = std::make_shared<ShaderUniforms::Matrix44Uniform>("transformationMatrix", nullptr);
		m_DefaultRenderRoutine->AddUniform(prtc_DefaultRoutineValues.TransformationUniform);
		prtc_Routines.Add(m_DefaultRenderRoutine);

		prtc_Shader = std::shared_ptr<Core::Shader>(Core::Shader::Create(
			Core::ShaderSettings(
				"D:/Programming/Test files/nfw/shaders/triton/v4.shader", 
				"D:/Programming/Test files/nfw/shaders/triton/f4.shader")));

		prtc_EventManager = std::make_unique<Core::EventManager>();

		#ifndef TR_DISABLE_GUI
			m_GUIContext = aSettings.ImGUIContext;
			ImGui::SetCurrentContext(m_GUIContext);
			UI::GUICollection::InitGUI(aSettings.WindowWidth, aSettings.WindowHeight);
			prtc_GUIS = std::make_unique<UI::GUICollection>(prtc_EventManager.get());
		#endif

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

		prtc_EventManager->Dispatch();

		Render();

		#ifndef TR_DISABLE_GUI
			prtc_GUIS->UpdateCollection(prtc_Delta);
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

	void Application::OnEvent(Event* aEvent)
	{
		prtc_EventManager->Post(aEvent);
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