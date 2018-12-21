#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Events\EventManager.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Triton {

	Application::Application()
	{
		prtc_Display = std::unique_ptr<Core::Display>(Core::Display::Create(Core::DisplaySettings()));
		TR_GLEW_INIT;

		prtc_Shader = std::unique_ptr<Core::Shader>(Core::Shader::Create(
			Core::ShaderSettings(
				"D:/Programming/Test files/nfw/shaders/triton/v1.shader", 
				"D:/Programming/Test files/nfw/shaders/triton/f1.shader")));

		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());

		glViewport(0, 0, prtc_Display->GetWidth(), prtc_Display->GetHeight());
	}
	
	Application::~Application()
	{
	}

	void Application::Run()
	{	
		glEnable(GL_DEPTH_TEST);

		prtc_Shader->Enable();
		Matrix44 ProjMatrix = Core::CreateProjectionMatrix(prtc_Display->GetWidth(), prtc_Display->GetHeight(), 70, 2000.0f, 0.1f);
		prtc_Shader->SetUniform("projectionMatrix", ProjMatrix);		

		PreExecutionSetup();
		while (!prtc_Display->Closed())
		{			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OnUpdate();

			Core::EventManager::Dispatch();
			prtc_EntityRegistry->UpdateSystems();
			prtc_Display->OnUpdate();
		}
	}
}