#pragma once

#include "TRMacros.h"
#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Core\Display.h"
#include "Core\Shader\Shader.h"
#include "Entity\Registry.h"

namespace Triton {

	//Main class that allows for project creation
	class TRITON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	protected:
		virtual Core::Shader* CreateShader() = 0;
	protected:
		std::unique_ptr<Core::Display> prtc_Display;
		Core::Shader* prtc_Shader;
		std::unique_ptr<ECS::Registry> prtc_EntityRegistry;
	private:

	};

	Application* CreateApplication();
}