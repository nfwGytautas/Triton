#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Events\EventManager.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"

#include <chrono>

namespace Triton {

	Application::Application(const AppSettings& aSettings)
		: EventManager(), EventListener(this)
	{
		// Init graphics, create context and set up event callbacks
		Context = Impl::createContext(aSettings);

		Context->init();
		Context->window->create(aSettings.WindowWidth, aSettings.WindowHeight);
		Context->setContextEventCallBacks(this);
		Context->init_additional();

		m_SceneManager = new Manager::SceneManager(Context);

		// Set up relay ptrs
		SceneManager = m_SceneManager;
	}
	
	Application::~Application()
	{
		Impl::destroyContext(Context);

		delete m_SceneManager;
	}

	void Application::setup()
	{
		Context->renderer->recalc_matrices();
		PreExecutionSetup();
	}

	void Application::frame()
	{	
		static std::chrono::high_resolution_clock timer;
		using ms = std::chrono::duration<float, std::milli>;
		auto start = timer.now();


		Context->renderer->newFrame(0.0f, 0.5f, 0.5f, 0.0f);

		Context->renderer->default(); // Reset the renderer to it's default state
		
		Dispatch();
		
		OnUpdate();	
		
		Render();
		
		Context->renderer->endFrame();
		
		Context->update();
		
		
		auto stop = timer.now();
		prtc_Delta = std::chrono::duration_cast<ms>(stop - start).count() / 1000.0f;
	}

	bool Application::shouldClose()
	{
		return Context->window->windowClosed();
	}

	void Application::OnEvent(Event* aEvent)
	{
		Post(aEvent);
	}

	void Application::Restart()
	{
		PreExecutionSetup();
	}
}