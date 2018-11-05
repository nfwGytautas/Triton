#include "Application.h"

#include <stdio.h>

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
		while(true) 
			printf("Running!\n");
	}

	void Application::PrintInformation()
	{
		printf("%s \n", Renderer->RendererInformation());
	}

	Systems::Renderer2D* Application::CreateRenderer()
	{
		return new Systems::TridentRenderer();
	}

}