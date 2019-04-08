#pragma once

#include "TritonShell\Config.h"

#include "Entity\Registry.h"
#include "Entity\Components\TritonComponents.h"

#include "Triton\Application.h"
#include "Triton\Events\Events.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

#include "TritonShell\Camera\Camera.h"


namespace Triton {

	//Class that inherits core application and extends it with some functionality
	class ShellApplication : 
		public Application, 
		protected Triton::EventListener
	{
	public:
		ShellApplication(const Triton::AppSettings& aSettings);
		virtual ~ShellApplication();

		virtual void Execute() override;
	protected:
		virtual void Render() override;
		virtual void PreExecutionSetup() = 0;
		virtual void OnUpdate() = 0;
		virtual void FixedTimeOnUpdate() {}

		void RunShell();
		void RestartShell();
	protected:
		std::shared_ptr<Camera> prtc_Camera;
		std::shared_ptr<ECS::Registry> prtc_EntityRegistry;

		TR_EDITOR_TOOLS
	};
}