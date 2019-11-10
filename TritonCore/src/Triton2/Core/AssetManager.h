#pragma once

#include "TritonTypes/Asset.h"
#include "TritonTypes/AssetDictionary.h"

namespace Triton
{
	namespace Graphics
	{
		// Forward declaration
		class Context;
	}

	namespace Core
	{
		/**
		 * Class to keep engine assets in one place
		 */
		class AssetManager
		{
		public:
			using AssetAddedCallback = std::function<void(reference<Asset>)>;
		public:
			/**
			 * Create an asset map instance with a callback
			 *
			 * @param addedCallback Callback for added assets
			 */
			AssetManager(AssetAddedCallback addedCallback);

			/**
			 * Create an asset map instance with a callback and a dictionary
			 *
			 * @param addedCallback Callback for added assets
			 * @param dictionary The dictionary that will be used by the asset manager 
			 * when loading by name
			 */
			AssetManager(AssetAddedCallback addedCallback, reference<AssetDictionary> dictionary);

			/**
			 * Cleans up and then destroys the asset manager
			 */
			~AssetManager();

			/**
			 * Unloads specified assets, meaning that they are removed from the map
			 * The assets will be deleted once all the references go out of scope
			 *
			 * @param assets Vector of asset names that should be unloaded
			 */
			void unloadAssets(const std::vector<std::string>& assets);

			/**
			 * Unloads all assets
			 */
			void unloadAll();

			/**
			 * Unloads all assets that whose names are not in the specified vector
			 *
			 * @param assets Vector of asset names that should be unloaded
			 */
			void unloadExcept(const std::vector<std::string>& assets);

			/**
			 * Unloads all unused (ref count is '1') assets
			 */
			void unloadUnused();

			/**
			 * Check if an asset with a name already exists
			 *
			 * @param name Name of the asset
			 * @return True if the asset manager has loaded an asset with the given name
			 */
			bool hasAsset(const std::string& name) const;

			/**
			 * Check if an asset with a name already exists
			 *
			 * @param name Name of the asset
			 * @return Does the map contain an asset with the specified name
			 */
			bool operator==(const std::string& name) const
			{
				return hasAsset(name);
			}

			/**
			 * [Multi threaded]
			 * Loads an asset with the specified name. The asset needs an entry inside an asset dictionary
			 * and that dictionary must be loaded into the engine.
			 * If multiple dictionaries contains the same asset name entry the latest added one will be used
			 *
			 * @param name Name of the asset to be loaded
			 */
			void loadAssetByName(const std::string& name);

			/**
			 * [Convenience]
			 * [Multi threaded]
			 * Same as calling loadAssetByName(name) with each vector entry
			 *
			 * @param names Names of the asset to be loaded
			 */
			void loadAssetsByName(const std::vector<std::string>& names);

			/**
			 * [Convenience] 
			 * [Multi threaded]
			 * Same as calling IO::loadAssetFromDisk() and then addAsset() with the loaded asset
			 * Consider using loading by name together with a dictionary
			 *
			 * @param file File to the asset
			 */
			void loadAsset(const std::string& file);

			/**
			 * [Convenience]
			 * [Multi threaded]
			 * Same as calling loadAsset(file) with each vector entry
			 *
			 * @param files Files to the assets
			 */
			void loadAssets(const std::vector<std::string>& files);

			/**
			 * [Thread safe, uses mutex]
			 * Add an asset to the manager
			 *
			 * @param asset Asset to add
			 */
			void addAsset(reference<Asset> asset);

			/**
			 * [Thread safe, uses mutex]
			 * Get the asset with the specified name
			 *
			 * @param name Name of the asset
			 * @return Reference to an asset with the specified name
			 */
			reference<Asset> getAsset(const std::string& name) const;

			/**
			 * Same as getAsset()
			 *
			 * @param name Name of the asset
			 * @return Reference to an asset with the specified name
			 */
			reference<Asset> operator[](const std::string& name) const
			{
				return getAsset(name);
			}

			/**
			 * Wait for asset manager to finish all asset loading
			 */
			void wait();

			/**
			 * Wait for asset manager to finish loading an asset with specified name
			 * for a specified amount of time in seconds.
			 * If an asset with the specified name can't be found after amount of time
			 * has passed the return value is a reference to nullptr
			 *
			 * @param name Name of the asset to wait for
			 * @param amount The amount to wait for if 0 then the function will wait forever
			 * @return The asset that was waited for
			 */
			reference<Asset> waitFor(const std::string& name, unsigned int amount);
		private:
			class AssetManagerImpl;
			AssetManagerImpl* m_impl;
		};
	}
}