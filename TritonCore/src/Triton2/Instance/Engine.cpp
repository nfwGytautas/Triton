#include "TRpch.h"
#include "Engine.h"

#include "Triton2/Utility/Log.h"

#include "TritonPlatform2/CrossTypes/Core/Context.h"
#include "TritonPlatform2/CrossTypes/Core/AudioContext.h"

#include "Triton2/Core/AssetManager.h"
#include "Triton2/Core/SceneManager.h"

#include "TritonTypes/Asset.h"

#include "Triton2/Instance/EngineAssets.h"

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

		// Setup default assets
		setupDefaultAssets();
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

	void Engine::setupDefaultAssets()
	{
		IO::MeshData* quadData = new IO::MeshData();

		IO::Mesh quadMesh;
		quadMesh.DynamicBuffer = 0;
		quadMesh.Stride = 3 + 2;
		quadMesh.vertices = 
		{
			// Bottom left.
			-1.0f, -1.0f, 1.0f,
			0.0f, 1.0f,
		
			// Top left
			-1.0f, 1.0f, 1.0f,
			0.0f, 0.0f,
		
			// Top right.
			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f,
		
			// Bottom right.
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f,
		};
		
		quadMesh.indices = {0, 1, 2, 0, 2, 3};

		quadData->meshes.push_back(quadMesh);
		reference<MeshAsset> _2dquad = new MeshAsset("2DQuad", quadData);
		m_assets->addAsset(_2dquad.as<Asset>());


		IO::ShaderData* shaderData = new IO::ShaderData();
		shaderData->source = Predefined::_2dshaderSource;
		shaderData->vertexEntry = "vertex_quad";
		shaderData->pixelEntry = "pixel_quad";

		for (int flag : {Flags::sFlag_Matrices, Flags::sFlag_NoNormals})
		{
			shaderData->flags.set(flag);
		}

		reference<ShaderAsset> _2dshader = new ShaderAsset("2DShader", shaderData);
		m_assets->addAsset(_2dshader.as<Asset>());
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