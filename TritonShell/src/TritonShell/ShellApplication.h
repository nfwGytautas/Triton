#pragma once

#include "TritonShell\Config.h"

#include "Triton\Application.h"
#include "Triton\Events\Events.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

#include "TritonShell\Entity\Registry.h"
#include "TritonShell\Entity\Systems\TritonSystems.h"

#include "TritonShell\Camera\Camera.h"

namespace Triton {

	//Class that inherits core application and extends it with some functionality
	class ShellApplication : 
		TR_SCRIPTING_INTERFACE
		public Application, 
		protected Triton::EventListener
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
		std::shared_ptr<ECS::Registry> prtc_EntityRegistry;

		std::unique_ptr<Triton::Systems::BatchSystem> prtc_BatchSystem;

		TR_EDITOR_TOOLS
	};
}