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

		// Create the engine wide dictionary
		m_dictionary = reference<Core::AssetDictionary>(new Core::AssetDictionary());

		// Create a graphics context
		m_context = Graphics::Context::create();
		m_context->init(settings);

		// Create asset manager
		m_assets = new Core::AssetManager(m_context, m_dictionary);

		// Create scene manager using asset manager
		m_scenes = new Core::SceneManager(m_assets, m_dictionary);
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

	void Engine::loadDictionary(const std::string& dict)
	{
		TR_SYSTEM_DEBUG("Loading dictionary: '{0}'", dict);

		Core::AssetDictionary* dictionary = Core::AssetDictionary::loadFromFile(dict);
		m_dictionary->append(*dictionary);

		TR_SYSTEM_DEBUG("Engine filled with '{0}' new associations", dictionary->size());

		delete dictionary;
	}

	void Engine::addDictionary(const Core::AssetDictionary& dict)
	{
		m_dictionary->append(dict);
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