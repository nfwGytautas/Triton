#pragma once

#include "TritonPlatform\PlatformAbstraction.h"

#include "TRMacros.h"
#include "Triton\Config.h"
#include "Triton\Limits.h"

#include "TritonPlatform/mathematical.h"

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

#include "AppSettings.h"

namespace Triton 
{

	//Main class that allows for project creation
	class Application : 
		protected Core::EventReceiver,
		protected Core::EventManager,
		protected Triton::EventListener
	{
	public:
		Application(const AppSettings& aSettings);
		virtual ~Application();

		// Setup the engine and context
		void setup();

		// Update and render a single frame
		void frame();

		// Check if the engine has recieved a command to close
		bool shouldClose();
	protected:
		virtual void Render() = 0;
		virtual void PreExecutionSetup() = 0;
		virtual void OnUpdate() = 0;
		virtual void FixedTimeOnUpdate() {}

		void Restart();
	protected:
		virtual void OnEvent(Event* aEvent) override;
	protected:
		float prtc_Delta = 0.0f;

		
	protected:
		relay_ptr<Manager::SceneManager> SceneManager;

		PType::Context* Context;
	private:
		Manager::SceneManager* m_SceneManager;
	};

	Application* CreateApplication(Triton::AppSettings& aSettings);
	void Loop(Triton::Application* application);
}