#pragma once

#include "TRMacros.h"
#include "Event manager\Events.h"
#include "Event manager\EventManager.h"
#include "Core\Display.h"
#include "Core\Shader\Shader.h"

namespace Triton {

	//Main class that allows for project creation
	class TRITON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	protected:
		Core::Display* Display;
	};

	Application* CreateApplication();
}