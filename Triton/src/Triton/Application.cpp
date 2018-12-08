#include "Application.h"

#include "Event manager\EventManager.h"
#include "Event manager\ApplicationEvent.h"
#include "Event manager\KeyEvent.h"
#include "Core\GraphicsAPI.h"

namespace Triton {

	Application::Application()
	{
		Renderer = CreateRenderer();
		Display = new Core::Display();
	}

	Application::~Application()
	{
		delete Renderer;
		delete Display;
	}

	void Application::Run()
	{
		while (!Display->Closed())
		{
			Display->Update();
			glfwPollEvents();
		}
	}

	void Application::PrintInformation()
	{
		TR_CORE_INFO("{0}",Renderer->RendererInformation());
	}

	Systems::Renderer* Application::CreateRenderer()
	{
		return new Systems::TridentRenderer();
	}

}