#pragma once

#include "TritonShell\Config.h"

#include "Entity\Registry.h"
#include "Entity\Components\TritonComponents.h"

#include "Triton\Application.h"
#include "Triton\Events\Events.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

#include "TritonShell\Core\Scene.h"

#include "TritonShell\Camera\Camera.h"

#include "TritonShell\Serialize\Serialize.h"

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
		virtual void Render() final;
		virtual void PreExecutionSetup() = 0;
		virtual void OnUpdate() = 0;
		virtual void FixedTimeOnUpdate() {}

		void RunShell();
		void RestartShell();
	protected:
		std::unique_ptr<Scene> prtc_Scene;
	protected:
		TR_EDITOR_TOOLS
	};
}