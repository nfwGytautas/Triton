#pragma once

#include "Triton/Core/TritonClass.h"

#include "Triton/Managers/ManagerBase.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Core
	{
		class TritonCache : public TritonClass
		{
		public:
			// Check if an object is present in the cache returns that object if present else nullptr
			reference<PType::FactoryObject> checkFObject(Resource::AssetCreateParams& assetParams, bool& result);

			// Caches a platform object with specified asset create params
			void cacheFObject(Resource::AssetCreateParams& assetParams, reference<PType::FactoryObject> object);

		private:
			// Get the count of paths to check in the Paths array inside create params
			int getAssetPathCount(Resource::AssetCreateParams& assetParams);
		private:
			// Vector containing all the platform objects that are currently cached
			std::vector<reference<PType::FactoryObject>> m_cachedFactoryObjects;
			// All the entries of all cached objects
			std::vector<std::vector<std::string>> m_assetPaths;
		};
	}
}