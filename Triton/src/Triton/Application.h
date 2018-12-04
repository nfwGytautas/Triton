#pragma once

#include "TRMacros.h"
#include "Systems\Systems.h"
#include "Logger\Log.h"
#include "Event manager\Events.h"
#include "Event manager\EventManager.h"

namespace Triton {

	//Main class that allows for project creation
	class TRITON_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		virtual void PrintInformation();
	protected:
		Systems::Renderer* Renderer;

		virtual Systems::Renderer* CreateRenderer();
	};

	Application* CreateApplication();
}