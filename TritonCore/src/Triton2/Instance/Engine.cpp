#include "TRpch.h"
#include "Engine.h"

#include "Triton2/Utility/Log.h"

#include "TritonPlatform2/CrossTypes/Core/Context.h"

#include "Triton2/Core/AssetManager.h"
#include "Triton2/Core/SceneManager.h"

namespace Triton
{
	void Engine::init(EngineSettings& settings)
	{
		// Initialize logging first
		Log::init();

		// Create a graphics context
		m_context = Graphics::Context::create();
		m_context->init(settings);

		// Create asset manager
		m_assets = new Core::AssetManager(m_context);

		// Create scene manager using asset manager
		m_scenes = new Core::SceneManager(m_assets);
	}

	void Engine::shutdown()
	{
		// Free all pointers in correct order

		// Automatically frees resources
		delete m_scenes;

		// Automatically frees resources
		delete m_assets;

		// Shutdown context and delete
		m_context->shutdown();
		delete m_context;

		// Flush log results
		Log::flush();
	}

	Graphics::Context& Engine::context() const
	{
		return *m_context;
	}

	Core::AssetManager& Engine::assets() const
	{
		return *m_assets;
	}

	Core::SceneManager& Engine::scenes() const
	{
		return *m_scenes;
	}

	Engine& Engine::getInstance()
	{
		static Engine instance;
		return instance;
	}
	
	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}
}