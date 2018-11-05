#pragma once

#include "TRMacros.h"

#include "Systems\Systems.h"

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
		Systems::Renderer2D* Renderer;

		virtual Systems::Renderer2D* CreateRenderer();
	};

	Application* CreateApplication();
}