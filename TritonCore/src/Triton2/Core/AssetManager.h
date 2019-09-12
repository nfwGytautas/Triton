#pragma once

#include "TritonPlatform2/CrossTypes/Base.h"
#include "TritonPlatform2/CrossTypes/Core/Context.h"
#include "TritonTypes/Asset.h"

namespace Triton
{
	namespace Core
	{
		/**
		 * Class to keep engine assets in one place
		 */
		class AssetManager
		{
		public:
			/**
			 * Create an asset map instance with the graphics context
			 *
			 * @param context The graphics Context
			 */
			AssetManager(Graphics::Context* context);

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
			 * @return Does the map contain an asset with the specified name
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
			 * [Convenience] 
			 * Same as calling IO::loadAssetFromDisk() and then addAsset() with the loaded asset
			 *
			 * @param file File to the asset
			 * @return Reference to a loaded asset
			 */
			reference<Asset> loadAsset(const std::string& file);

			/**
			 * Add an asset to the manager
			 *
			 * @param asset Asset to add
			 */
			void addAsset(reference<Asset> asset);

			/**
			 * Get the asset with the specified name
			 *
			 * @param name Name of the asset
			 * @return Reference to an asset with the specified name
			 */
			reference<Asset> getAsset(const std::string& name) const;

			/**
			 * Load a file from disk and
			 *
			 * @param name Name of the asset
			 * @return Reference to an asset with the specified name
			 */
			reference<Asset> operator[](const std::string& name) const
			{
				return getAsset(name);
			}
		private:
			/// Map of asset name to asset reference
			std::vector<reference<Asset>> m_assets;
			
			/// Pointer to the context instance
			Graphics::Context* m_contextInstance;
		};
	}
}