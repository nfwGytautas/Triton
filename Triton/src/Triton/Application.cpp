#include "Application.h"

#include "Event manager\EventManager.h"
#include "Event manager\ApplicationEvent.h"
#include "Event manager\KeyEvent.h"

namespace Triton {

	Application::Application()
	{
		Renderer = CreateRenderer();
	}

	Application::~Application()
	{
		delete Renderer;
	}

	void Application::Run()
	{
		WindowResizeEvent wre(100,100);
		Core::EventManager::Post(wre);
		KeyPressedEvent kpe(15, 2);
		Core::EventManager::Post(kpe);
		Core::EventManager::Dispatch();
		while (true)
		{

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