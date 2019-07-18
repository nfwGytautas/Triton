#pragma once

#include "TritonTypes\reference.h"
#include "TritonTypes\relay_ptr.h"

#include "TritonPlatform\PlatformAbstraction.h"
#include "Triton\Core\RenderBuffer.h"

#include "TRMacros.h"
#include "Triton\Config.h"
#include "Triton\Limits.h"

#include "TritonPlatform/mathematical.h"

#include "File\File.h"

#include "Core\Input\InputManager.h"
#include "Events\Events.h"
#include "Events\EventManager.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"

#include "TritonTypes\data_collection.h"

#include "Core\Graphics\Lighting\Light.h"
#include "Core\Graphics\Lighting\PointLight.h"
#include "Core\Graphics\Lighting\DirectionalLight.h"
#include "Core\Graphics\Lighting\SpotLight.h"

#include "Entity\Registry.h"
#include "Entity\Components\TritonComponents.h"

#include "Managers/SceneManager.h"

#include "Core/Wrapers/ShaderProgram.h"
#include "Core/Wrapers/Material.h"
#include "Core/Wrapers/Viewport.h"

#include "Core\Scene.h"
#include "Camera\Camera.h"
#include "Serialize\Serialize.h"

#include "AppSettings.h"

namespace Triton 
{

	//Main class that allows for project creation
	class Application :
		protected Triton::EventListener
	{
	public:
		Application(const AppSettings& aSettings);
		virtual ~Application();

		// Run the application automatically called from the EntryPoint.h
		void Execute();

		// Setup the engine and context
		void setup();

		// Update and render a single frame
		void frame();

		// Check if the engine has recieved a command to close
		bool shouldClose();

		// Tells the engine to render the specified scene
		void renderScene(reference<Scene>& scene, reference<Data::Viewport>& renderTo, bool clearFBO = true);

		// Calls the scene render method with required parameters
		void renderCustomScene(reference<SceneBase>& scene);
	protected:
		virtual void Render() = 0;
		virtual void RenderOnTop() {}
		virtual void PreExecutionSetup() = 0;
		virtual void OnUpdate() = 0;
		virtual void FixedTimeOnUpdate() {}

		void Restart();

	private:
		// Update a single frame
		void updateMT();

		// Render a single frame
		void renderMT();
	protected:
		float prtc_Delta = 0.0f;	
		float prtc_UpdateDelta = 0.0f;
		float prtc_RenderDelta = 0.0f;
	protected:
		relay_ptr<Manager::SceneManager> SceneManager;
		relay_ptr<Core::InputManager> Input;

		PType::Context* Context;
	private:
		Manager::SceneManager* m_SceneManager;
		Core::InputManager* m_iManager;


		// Used for multi-threaded engine execution
		Core::RenderBuffer* m_RenderBuffer1;
		Core::RenderBuffer* m_RenderBuffer2;

		Core::RenderBuffer* m_CurrentRenderBuffer;
		Core::RenderBuffer* m_CurrentUpdateBuffer;

		// Threads
		std::thread* m_renderThread;

		bool m_doneRendering = true;
	};
}