#pragma once

#include "Core\GraphicsAPI.h"

#include "Event manager\Events.h"
#include "Event manager\EventManager.h"

extern Triton::Application* Triton::CreateApplication();

int main(int argc, char** argv)
{
	//Initialize logger
	Triton::Log::Init();

	//Initialize graphics api
	if (Triton::Core::GraphicsAPI::Initialize())
	{		
		//Create triton application by user defined procedure
		auto app = Triton::CreateApplication();

		//Output app information
		app->PrintInformation();

		//Run the created application
		app->Run();

		//Delete the created triton application
		delete app;
	}
}