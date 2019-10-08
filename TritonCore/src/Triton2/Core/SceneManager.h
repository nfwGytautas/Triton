#pragma once

#include "Triton2/Scene/Scene.h"
#include "TritonTypes/AssetDictionary.h"

namespace Triton
{
	namespace Core
	{
		// Forward declaration
		class AssetManager;

		/**
		 * Class to keep scenes in one place
		 */
		class SceneManager
		{
		public:
			/**
			 * Create a scene manager instance that uses the specified AssetManager
			 *
			 * @param assetManager The asset manager instance that will be used by the scene manager
			 */
			SceneManager(AssetManager* assetManager);

			/**
			 * Create a scene manager instance that uses the specified AssetManager and dictionary
			 *
			 * @param assetManager The asset manager instance that will be used by the scene manager
			 * @param dictionary The dictionary that will be used by the scene manager 
			 * when loading by name
			 */
			SceneManager(AssetManager* assetManager, reference<AssetDictionary> dictionary);

			/**
			 * Cleans up and then destroys the scene manager
			 */
			~SceneManager();

			/**
			 * Returns the currently active scene
			 *
			 * @return Reference to the active scene object
			 */
			reference<Scene> currentScene();

			/**
			 * Sets the currently active scene and unloads the previous scene if necessary
			 *
			 * @param scene Reference to the scene object that should be activated
			 * @param unloadPrevious If true unloads the previous scene from memory, true by default
			 */
			void setScene(reference<Scene> scene, bool unloadPrevious = true);

			/**
			 * [Convenience] 
			 * Same as calling setScene(getScene(name))
			 *
			 * @param scene Name of the scene to set
			 * @param unloadPrevious If true unloads the previous scene from memory, true by default
			 */
			void setScene(const std::string& scene, bool unloadPrevious = true);

			/**
			 * Unloads all scenes except the current one
			 */
			void unloadAll();

			/**
			 * Check if a scene with a name is loaded
			 *
			 * @param name Name of the scene
			 * @return Does the map contain a scene with the specified name
			 */
			bool hasScene(const std::string& name) const;
			
			/**
			 * [Multi threaded]
			 * Loads a scene with the specified name. The scene needs an entry inside an asset dictionary
			 * and that dictionary must be loaded into the engine.
			 * If multiple dictionaries contains the same scene name entry the latest added one will be used
			 *
			 * @param name Name of the scene to be loaded
			 */
			void loadSceneByName(const std::string& name);

			/**
			 * [Convenience] 
			 * [Multi threaded]
			 * Same as calling IO::loadSceneFromDisk() and then addScene() with the loaded scene
			 * Also queues up asset loading to asset manager
			 * Consider using loading by name together with a dictionary
			 *
			 * @param file File to the scene
			 */
			void loadScene(const std::string& file);

			/**
			 * [Thread safe, uses mutex]
			 * Add a scene to the manager
			 *
			 * @param scene Scene to add
			 */
			void addScene(reference<Scene> scene);

			/**
			 * [Thread safe, uses mutex]
			 * Get the scene with the specified name
			 *
			 * @param name Name of the scene
			 * @return Reference to a scene with the specified name
			 */
			reference<Scene> getScene(const std::string& name) const;

			/**
			 * Same as getScene()
			 *
			 * @param name Name of the scene
			 * @return Reference to a scene with the specified name
			 */
			reference<Scene> operator[](const std::string& name) const
			{
				return getScene(name);
			}

			/**
			 * Wait for scene manager to finish all scene loading
			 */
			void wait();

			/**
			 * Wait for scene manager to finish loading a scene with specified name
			 * for a specified amount of time in seconds.
			 * If a scene with the specified name can't be found after amount of time
			 * has passed the return value is a reference to nullptr
			 *
			 * @param name Name of the scene to wait for
			 * @param amount The amount to wait for if 0 then the function will wait forever
			 * @return The scene that was waited for
			 */
			reference<Scene> waitFor(const std::string& name, unsigned int amount);
		private:
			class SceneManagerImpl;
			SceneManagerImpl* m_impl;
		};
	}
}