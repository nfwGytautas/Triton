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
		m_Context = Impl::createContext();

		m_Context->init();
		m_Context->window->create(aSettings.WindowWidth, aSettings.WindowHeight);
		m_Context->setContextEventCallBacks(this);

		m_SceneManager = new Manager::SceneManager(m_Context);

		// Set up relay ptrs
		Context = m_Context;
		SceneManager = m_SceneManager;
	}
	
	Application::~Application()
	{
		Impl::destroyContext(m_Context);
	}

	void Application::Run()
	{	
		static std::chrono::high_resolution_clock timer;
		using ms = std::chrono::duration<float, std::milli>;
		auto start = timer.now();


		m_Context->renderer->default(); // Reset the renderer to it's default state
		m_Context->window->clear(0.0f, 0.5f, 0.5f, 0.0f); // Clear the window for rerendering

		Dispatch();

		OnUpdate();	

		Render();

		m_Context->update();


		auto stop = timer.now();
		prtc_Delta = std::chrono::duration_cast<ms>(stop - start).count() / 1000.0f;
	}

	Matrix44 Application::GetProjectionMatrix()
	{
		auto[width, height] = m_Context->window->getWindowSize();
		m_Context->setViewPort(0, 0, width, height);
		return Triton::Core::CreateProjectionMatrix(width, height, 45.0f, 0.1f, 100.0f);
	}

	void Application::OnEvent(Event* aEvent)
	{
		Post(aEvent);
	}

	void Application::Execute()
	{
		GetProjectionMatrix();

		PreExecutionSetup();
		while (!m_Context->window->windowClosed())
		{
			Run();
		}
	}

	void Application::Restart()
	{
		PreExecutionSetup();
	}
}