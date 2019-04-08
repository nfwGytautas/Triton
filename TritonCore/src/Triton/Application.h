#pragma once

#include "TRMacros.h"
#include "Triton\Config.h"
#include "Triton\Core\Platform.h"

#include "Core\Math\Math.h"

#include "Core\Display.h"
#include "Core\Shader\Shader.h"
#include "Core\Renderer\Renderer.h"

#include "File\File.h"

#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\EventReceiver.h"

#include "Core\Data\Mesh.h"
#include "Core\Data\Material.h"
#include "Core\Renderer\RenderRoutine.h"
#include "Core\Data\DataCollection.h"

namespace Triton {

	struct AppSettings
	{
		unsigned int WindowWidth = 600;
		unsigned int WindowHeight = 600;
		std::string WindowTitle = "Triton display";

		TR_GUI_CONTEXT(ImGUIContext)
	};

	//Main class that allows for project creation
	class Application : 
		protected Core::EventReceiver
	{
	public:
		struct DefaultRoutineValues
		{
			std::shared_ptr<ShaderUniforms::Matrix44Uniform> TransformationUniform;
		};

	public:
		Application(const AppSettings& aSettings);
		virtual ~Application();

		virtual void Execute();

	protected:
		virtual void Render() = 0;
		virtual void PreExecutionSetup() = 0;
		virtual void OnUpdate() = 0;
		virtual void FixedTimeOnUpdate() {}

		void Run();
		void UpdateProjectionMatrix();

		std::shared_ptr<Core::RenderRoutine> DefaultRenderRoutine()
		{
			return m_DefaultRenderRoutine;
		}

	protected:
		virtual void OnEvent(Event* aEvent) override;
	protected:
		float prtc_Delta = 0.0f;

		DefaultRoutineValues prtc_DefaultRoutineValues;


		Core::DataCollection<Data::Mesh> prtc_Meshes;
		Core::DataCollection<Data::Material> prtc_Materials;
		Core::DataCollection<Core::RenderRoutine> prtc_Routines;
	protected:
		TR_GUI_IMPLEMENTATION

		std::unique_ptr<Core::Display> prtc_Display;
		std::unique_ptr<Core::Renderer> prtc_Renderer;
		std::shared_ptr<Core::Shader> prtc_Shader;
		std::shared_ptr<Core::EventManager> prtc_EventManager;
	private:
		std::shared_ptr<Core::RenderRoutine> m_DefaultRenderRoutine;
		float m_LastFrame = 0.0f;
	};

	Application* CreateApplication(Triton::AppSettings& aSettings);
}