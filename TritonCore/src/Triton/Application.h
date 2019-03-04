#pragma once

#include "TRMacros.h"

#include "Core\Math\Math.h"

#include "Core\Display.h"
#include "Core\Shader\Shader.h"
#include "Core\Renderer\Renderer.h"
#include "Core\Camera.h"

#include "File\File.h"

#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"

#include "Core\Data\Structures\RenderBatch.h"
#include "Core\Data\Structures\Mesh.h"
#include "Core\Data\Structures\Material.h"

#include "GUI\GUICollection.h"

namespace Triton {

	struct AppSettings
	{

#ifndef TR_DISABLE_GUI
		ImGuiContext* ImGUIContext;
#endif
	};

	//Main class that allows for project creation
	class TRITON_API Application
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
	protected:
		float prtc_Delta = 0.0f;

		Data::RenderOrder* prtc_RenderOrder;
	protected:
#ifndef TR_DISABLE_GUI
		std::unique_ptr<UI::GUICollection> prtc_GUIS;
	private:
		ImGuiContext* mGUIContext;
	protected:
#endif
		std::unique_ptr<Core::Display> prtc_Display;
		std::unique_ptr<Core::Shader> prtc_Shader;
		std::unique_ptr<Core::Renderer> prtc_Renderer;
	private:
		float m_LastFrame = 0.0f;
	};

	Application* CreateApplication(const Triton::AppSettings& aSettings);
}