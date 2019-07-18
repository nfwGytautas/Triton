#pragma once

#include "Triton/Core/TritonClass.h"

#include "Triton/Managers/ManagerBase.h"
#include "Triton/Core/Asset/Asset.h"

#include "Triton/Core/Wrapers/Mesh.h"
#include "Triton/Core/Wrapers/Material.h"
#include "Triton/Core/Wrapers/Image.h"
#include "Triton/Core/Wrapers/ShaderProgram.h"
#include "Triton/Core/Wrapers/Viewport.h"

namespace Triton
{
	namespace Manager
	{
		class AssetManager : public Core::TritonClass
		{
		public:
			AssetManager();
			virtual ~AssetManager();

			// Creates an asset from the specified parameters and returns the created asset
			// that can be casted to whatever you want using the as() function of the reference object
			reference<Resource::Asset> createAsset(Resource::AssetCreateParams& params);

			// Returns an asset by it's id
			reference<Resource::Asset> getAssetByID(size_t id);

			// Frees an asset by it's id
			// NOTE: The asset might not be actually deleted if it's used somewhere else since all
			//		 assets are returned as reference objects.
			void freeAssetByID(size_t id);

#if TR_STRING_REPRESENTATIONS == 1
			// If string representation is enabled then the method returns an asset using it's name
			reference<Resource::Asset> getAssetByName(std::string name);

			// If string representation is enabled then the method frees an asset using it's name
			// NOTE: The asset might not be actually deleted if it's used somewhere else since all
			//		 assets are returned as reference objects.
			void freeAssetByName(std::string name);
#endif

			// Deletes all assets and frees all the asociated memory
			void cleanUp();
		private:
			// Checks if an asset of id can be returned safely
			bool checkReturnability(size_t id);

			// Adds asset to the created asset vector and to the map
			void addObject(reference<Resource::Asset>& asset);

			// Returns the next avaivable id for an asset
			size_t getNextID();
		private:
			// Vector containing all the assets that the manager has
			std::vector<reference<Resource::Asset>> m_createdAssets;

			// Map containing the ids of created assets and their instances
			std::unordered_map<size_t, reference<Resource::Asset>> m_IDAssetPairs;

			// The id that will be given to the next asset that gets created
			size_t m_nextAssetID = 0;
		};
	}
}