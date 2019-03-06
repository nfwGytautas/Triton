#pragma once

#include "TritonScript.h"

#include "Triton\Application.h"
#include "Triton\Events\Events.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

#include "TritonShell\Entity\Registry.h"
#include "TritonShell\Entity\Systems\TritonSystems.h"

#include "Triton\Core\Data\Structures\RenderBatch.h"

#include "TritonShell\Tools\GUIConsole.h"

namespace Triton {

	//Main class that allows for project creation
	class ShellApplication : 
		public Application, 
		protected Triton::EventListener,
#ifndef TR_DISABLE_SCRIPTING 
		protected Triton::Scripting::TRPythonScriptingInterface 
#endif
	{
	public:
		ShellApplication(const Triton::AppSettings& aSettings);
		virtual ~ShellApplication();

		virtual void Execute() override;
	protected:
		virtual void PreExecutionSetup() {}
		virtual void OnUpdate() {}
		virtual void FixedTimeOnUpdate() {}

		void RunShell();
		void RestartShell();
	protected:
		std::shared_ptr<Camera> prtc_Camera;
		std::unique_ptr<ECS::Registry> prtc_EntityRegistry;

		std::unique_ptr<Triton::Systems::BatchSystem> prtc_BatchSystem;

#ifndef TR_DISABLE_EDITOR_TOOLS
		std::shared_ptr<Tools::GUIConsole> prtc_Console;
#endif
	};
}