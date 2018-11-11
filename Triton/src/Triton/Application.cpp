#include "Application.h"

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