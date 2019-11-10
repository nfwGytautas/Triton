#pragma once

#include "TritonTypes/Settings.h"

#include "TritonTypes/AssetDictionary.h"

namespace Triton
{
	// Forward declarations
	class Asset;

	namespace Graphics
	{
		class Context;
	}

	namespace Audio
	{
		class AudioContext;
	}

	namespace Core
	{
		class AssetManager;
		class SceneManager;
	}	

	/**
	 * [Singleton]
	 * Singleton class that contains the entire functionality of the Triton engine
	 * Use this class to initialize and shutdown
	 * The instance of the engine can be accessed by calling getInstance() method
	 */
	class Engine
	{
	public:
		/**
		 * Initialize the Triton engine with specified parameter structure
		 *
		 * @param settings EngineSettings structure that contains initialization settings
		 */
		void init(EngineSettings& settings);

		/**
		 * Shutdown the Triton engine
		 */
		void shutdown();

		/**
		 * Returns engine's graphics context instance
		 *
		 * @return Context instance initialized by the engine
		 */
		Graphics::Context& graphicsContext() const;

		/**
		 * Returns engine's audio context instance
		 *
		 * @return Context instance initialized by the engine
		 */
		Audio::AudioContext& audioContext() const;

		/**
		 * Returns engine's asset manager instance
		 *
		 * @return AssetManager instance initialized by the engine
		 */
		Core::AssetManager& assets() const;

		/**
		 * Returns engine's scene manager instance
		 *
		 * @return SceneManager instance initialized by the engine
		 */
		Core::SceneManager& scenes() const;
	
		/**
		 * [Convenience]
		 * Same as calling addDictionary(*AssetDictionary::loadFromFile(dict))
		 * Loads an asset dictionary to be used for the scene manager
		 * When a scene is requested to be loaded the manager will look inside the
		 * dictionary to find the path to it's file path and then load it
		 *
		 * @param dict Path to the dictionary file
		 */
		void loadDictionary(const std::string& dict);

		/**
		 * Appends dictionary contents to the engine wide dictionary.
		 * Any duplicate entries are rewritten
		 *
		 * @param dict Dictionary to append
		 */
		void addDictionary(const Core::AssetDictionary& dict);

	private:
		/**
		 * Callback for the AssetManager to call when any assets have been added
		 * The engine adds to specific Context ThreadSynchronizers depending on the asset
		 *
		 * Audio -> audio context
		 * Else -> graphics context
		 */
		void assetAdded(reference<Asset> asset);
	private:
		/// Graphics context created by the engine
		Graphics::Context* m_context;

		/// Audio context created by the engine
		Audio::AudioContext* m_audioContext;

		/// Asset manager created by the engine
		Core::AssetManager* m_assets;

		/// Scene manager created by the engine
		Core::SceneManager* m_scenes;

		/// Engine wide dictionary used by the asset and scene managers to find resources
		reference<Core::AssetDictionary> m_dictionary;
	public: // Singleton stuff

		/**
		 * Get the instance of the engine
		 */
		static Engine& getInstance();

		/// Copy disabled
		Engine(Engine const&) = delete;
		/// Copy disabled
		void operator=(Engine const&) = delete;

	private:
		/// Disable public instantiation
		Engine();
		/// Disable public destruction
		~Engine();
	};
}