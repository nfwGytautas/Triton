#pragma once

#include "TritonTypes/Settings.h"

namespace Triton
{
	// Forward declarations
	namespace Graphics
	{
		class Context;
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
		Graphics::Context& context() const;

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
	
	private:
		Graphics::Context* m_context;
		Core::AssetManager* m_assets;
		Core::SceneManager* m_scenes;

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
		/// Disable stack destruction
		~Engine();
	};
}