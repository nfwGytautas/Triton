#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Event manager\EventManager.h"
#include "Event manager\ApplicationEvent.h"
#include "Event manager\KeyEvent.h"
#include "Core\GraphicsAPI.h"

namespace Triton {

	Application::Application()
	{
		Display = new Core::Display();
		Core::GraphicsAPI::SetupContextCallbacks();
	}

	Application::~Application()
	{
		delete Display;
	}

	void Application::Run()
	{
		while (!Display->Closed())
		{
			Display->Update();			

			Core::GraphicsAPI::PollCallbacks();
			Core::EventManager::Dispatch();
		}
	}

}