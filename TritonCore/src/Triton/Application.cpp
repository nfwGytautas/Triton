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
		m_Context = Impl::createContext(aSettings);

		m_Context->init();
		m_Context->window->create(aSettings.WindowWidth, aSettings.WindowHeight);
		m_Context->setContextEventCallBacks(this);
		m_Context->init_additional();

		m_SceneManager = new Manager::SceneManager(m_Context);

		// Set up relay ptrs
		Context = m_Context;
		SceneManager = m_SceneManager;
	}
	
	Application::~Application()
	{
		Impl::destroyContext(m_Context);
	}

	void Application::setup()
	{
		Context->renderer->recalc_projection();
		PreExecutionSetup();
	}

	void Application::frame()
	{	
		static std::chrono::high_resolution_clock timer;
		using ms = std::chrono::duration<float, std::milli>;
		auto start = timer.now();

		m_Context->renderer->newFrame(0.0f, 0.5f, 0.5f, 0.0f);

		m_Context->renderer->default(); // Reset the renderer to it's default state

		Dispatch();

		OnUpdate();	

		Render();

		m_Context->renderer->endFrame();

		m_Context->update();


		auto stop = timer.now();
		prtc_Delta = std::chrono::duration_cast<ms>(stop - start).count() / 1000.0f;
	}

	bool Application::shouldClose()
	{
		return m_Context->window->windowClosed();
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