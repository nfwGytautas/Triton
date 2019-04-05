#pragma once

#include "TRMacros.h"
#include "Triton\Config.h"
#include "Triton\Core\Platform.h"

#include "Core\Math\Math.h"

#include "Core\Display.h"
#include "Core\Shader\Shader.h"
#include "Core\Renderer\Renderable.h"

#include "File\File.h"

#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\EventReceiver.h"

#include "Core\Data\Mesh.h"
#include "Core\Data\Material.h"

namespace Triton {

	struct AppSettings
	{
		unsigned int WindowWidth = 600;
		unsigned int WindowHeight = 600;
		std::string WindowTitle = "Triton display";

		TR_GUI_CONTEXT(ImGUIContext)
	};

	//Main class that allows for project creation
	class TRITON_API Application : 
		protected Core::EventReceiver
	{
	public:
		Application(const AppSettings& aSettings);
		virtual ~Application();

		virtual void Execute();

	protected:
		virtual void PreExecutionSetup() {}
		virtual void OnUpdate() {}
		virtual void FixedTimeOnUpdate() {}

		void Run();
		void UpdateProjectionMatrix();

		size_t AddRenderable(std::shared_ptr<Core::Renderable> aRenderable);
		std::shared_ptr<Core::Renderable> TakeRenderable(size_t aRenderable);

	protected:
		virtual void OnEvent(Event* aEvent) override;
	protected:
		float prtc_Delta = 0.0f;
	protected:
		TR_GUI_IMPLEMENTATION

		std::unique_ptr<Core::Display> prtc_Display;
		std::shared_ptr<Core::Shader> prtc_Shader;
		std::shared_ptr<Core::EventManager> prtc_EventManager;		
	private:
		std::vector< std::shared_ptr<Core::Renderable>> m_Renderables;

		float m_LastFrame = 0.0f;
	};

	Application* CreateApplication(Triton::AppSettings& aSettings);
}