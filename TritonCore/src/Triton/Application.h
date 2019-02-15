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

#include "Entity\Registry.h"
#include "Entity\Components\TritonComponents.h"
#include "Entity\Systems\TritonSystems.h"

#include "Core\Data\Structures\RenderBatch.h"
#include "Core\Data\Structures\Mesh.h"
#include "Core\Data\Structures\Material.h"

#include "AppState\AppState.h"

namespace Triton {

	//Main class that allows for project creation
	class TRITON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Restart();
	protected:
		virtual void PreExecutionSetup() {}
		virtual void OnUpdate() {}
		virtual void FixedTimeOnUpdate() {}
	protected:
		float prtc_Delta = 0.0f;

		std::vector<Data::RenderBatch> prtc_v_RenderBatch;
	protected:
		std::shared_ptr<Camera> prtc_Camera;
		std::unique_ptr<Core::Display> prtc_Display;
		std::unique_ptr<Core::Shader> prtc_Shader;
		std::unique_ptr<ECS::Registry> prtc_EntityRegistry;
		std::unique_ptr<Core::Renderer> prtc_Renderer;
		std::unique_ptr<Storage::AppState> prtc_AppState;
	private:
		float m_LastFrame = 0.0f;
	};

	Application* CreateApplication();
}