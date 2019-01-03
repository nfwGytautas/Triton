#pragma once

#include "TRMacros.h"
#include "Core\Math\Math.h"
#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Core\Display.h"
#include "Core\Shader\Shader.h"
#include "Entity\Registry.h"
#include "Entity\Components\TritonComponents.h"
#include "Entity\Systems\TritonSystems.h"
#include "Core\Data\Structures\Mesh.h"
#include "Core\Data\Storage\MeshStorage.h"
#include "Core\Camera.h"
#include "Core\Renderer\Renderer.h"

namespace Triton {

	//Main class that allows for project creation
	class TRITON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	protected:
		virtual void PreExecutionSetup() {}
		virtual void OnUpdate() {}
	protected:
		float m_Delta = 0.0f;
	protected:
		std::unique_ptr<Core::Display> prtc_Display;
		std::unique_ptr<Core::Shader> prtc_Shader;
		std::unique_ptr<ECS::Registry> prtc_EntityRegistry;
		std::unique_ptr<Storage::MeshStorage> prtc_MainMeshStorage;
		std::unique_ptr<Core::Renderer> prtc_Renderer;
	private:
		float m_LastFrame = 0.0f;
	};

	Application* CreateApplication();
}