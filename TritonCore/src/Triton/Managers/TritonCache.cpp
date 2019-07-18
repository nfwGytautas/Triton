#include "TRpch.h"
#include "AssetManager.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/File/File.h"
#include "TritonCache.h"

namespace Triton
{
	namespace Core
	{
		reference<PType::FactoryObject> TritonCache::checkFObject(Resource::AssetCreateParams& assetParams, bool& result)
		{
			int countToCheck = getAssetPathCount(assetParams);

			if (countToCheck == 0)
			{
				result = false;
				return nullptr;
			}

			int index = -1;
			for (auto& paths : m_assetPaths)
			{
				index++;

				if (countToCheck != paths.size())
				{
					continue;
				}

				for (int i = 0; i < countToCheck; i++)
				{
					if (paths[i] != assetParams.Paths[i])
					{
						continue;
					}
				}

				result = true;
				return m_cachedFactoryObjects[index];
			}

			result = false;
			return nullptr;
		}

		void TritonCache::cacheFObject(Resource::AssetCreateParams& assetParams, reference<PType::FactoryObject> object)
		{
			if (getAssetPathCount(assetParams) == 0)
			{
				return;
			}

			m_cachedFactoryObjects.push_back(object);
			m_assetPaths.push_back(std::vector<std::string>(std::begin(assetParams.Paths), std::end(assetParams.Paths)));
		}

		int TritonCache::getAssetPathCount(Resource::AssetCreateParams& assetParams)
		{
			if (assetParams.Type == Resource::AssetCreateParams::AssetType::VIEWPORT)
			{
				return 0;
			}

			if (assetParams.Type == Resource::AssetCreateParams::AssetType::CUBEMAP)
			{
				return 6;
			}

			if (assetParams.Type == Resource::AssetCreateParams::AssetType::SHADER)
			{
				return 2;
			}
		}
	}
}