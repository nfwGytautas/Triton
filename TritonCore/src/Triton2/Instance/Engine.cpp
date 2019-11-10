#include "TRpch.h"
#include "Engine.h"

#include "Triton2/Utility/Log.h"

#include "TritonPlatform2/CrossTypes/Core/Context.h"
#include "TritonPlatform2/CrossTypes/Core/AudioContext.h"

#include "Triton2/Core/AssetManager.h"
#include "Triton2/Core/SceneManager.h"

#include "TritonTypes/Asset.h"

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

		// Create a audio context
		m_audioContext = Audio::AudioContext::create();
		m_audioContext->init(settings);

		// Create asset manager
		m_assets = new Core::AssetManager(std::bind(&Engine::assetAdded, this, std::placeholders::_1), m_dictionary);

		// Create scene manager using asset manager
		m_scenes = new Core::SceneManager(m_assets, m_dictionary);
	}

	void Engine::shutdown()
	{
		// Automatically frees resources
		delete m_scenes;

		// Automatically frees resources
		delete m_assets;

		// Shutdown context and delete
		m_context->shutdown();
		delete m_context;

		// Shutdown audio context and delete
		m_audioContext->shutdown();
		delete m_audioContext;

		// Flush log results
		Log::flush();
	}

	Graphics::Context& Engine::graphicsContext() const
	{
		return *m_context;
	}

	Audio::AudioContext& Engine::audioContext() const
	{
		return *m_audioContext;
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

	void Engine::assetAdded(reference<Asset> asset)
	{
		if (!asset->isCreated())
		{
			if (asset->type() != Asset::AssetType::AUDIO)
			{
				m_context->synchronizer().enqueue(
					[&, asset]()
				{
					asset->create(m_context);
				});
			}
			else
			{
				m_audioContext->synchronizer().enqueue(
					[&, asset]()
				{
					asset->create(m_audioContext);
				});
			}
		}
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