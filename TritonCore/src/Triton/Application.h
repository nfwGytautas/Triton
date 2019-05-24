#pragma once

#include "TritonPlatform\PlatformAbstraction.h"

#include "TRMacros.h"
#include "Triton\Config.h"
#include "Triton\Limits.h"

#include "TritonTypes/mathematical.h"

#include "File\File.h"

#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\EventReceiver.h"

#include "TritonTypes\data_collection.h"

#include "Core\Graphics\Lighting\Light.h"
#include "Core\Graphics\Lighting\PointLight.h"
#include "Core\Graphics\Lighting\DirectionalLight.h"
#include "Core\Graphics\Lighting\SpotLight.h"

#include "Entity\Registry.h"
#include "Entity\Components\TritonComponents.h"

#include "Managers/SceneManager.h"
#include "Core/Wrapers/Material.h"

#include "Core\Scene.h"
#include "Camera\Camera.h"
#include "Serialize\Serialize.h"

namespace Triton {

	struct AppSettings
	{
		unsigned int WindowWidth = 600;
		unsigned int WindowHeight = 600;
		std::string WindowTitle = "Triton display";
	};

	//Main class that allows for project creation
	class Application : 
		protected Core::EventReceiver,
		protected Core::EventManager,
		protected Triton::EventListener
	{
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
		void Restart();
		Matrix44 GetProjectionMatrix();
	protected:
		virtual void OnEvent(Event* aEvent) override;
	protected:
		float prtc_Delta = 0.0f;

		
	protected:
		relay_ptr<PType::Context> Context;
		relay_ptr<Manager::SceneManager> SceneManager;

	private:
		PType::Context* m_Context;
		Manager::SceneManager* m_SceneManager;
	};

	Application* CreateApplication(Triton::AppSettings& aSettings);
}