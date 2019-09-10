#include "TRpch.h"
#include "AssetManager.h"

#include "Triton2/Assert.h"
#include "TritonTypes/Asset.h"

#include "Triton2/File/File.h"

namespace Triton
{
	namespace Core
	{
		AssetManager::AssetManager(Graphics::Context* context)
			: m_contextInstance(context)
		{
		}

		void AssetManager::unloadAssets(const std::vector<std::string>& assets)
		{
			if (!assets.empty())
			{
				auto it = std::remove_if(m_assets.begin(), m_assets.end(),
					[&](const reference<Asset> asset)
				{
					return (std::find(assets.begin(), assets.end(), asset->getName()) != assets.end());
				});

				m_assets.erase(it, m_assets.end());
			}
		}

		void AssetManager::unloadAll()
		{
			m_assets.clear();
		}

		void AssetManager::unloadExcept(const std::vector<std::string>& assets)
		{
			if (!assets.empty())
			{
				auto it = std::remove_if(m_assets.begin(), m_assets.end(), 
					[&](const reference<Asset> asset) 
					{
						return !(std::find(assets.begin(), assets.end(), asset->getName()) != assets.end());
					});

				m_assets.erase(it, m_assets.end());
			}
		}

		void AssetManager::unloadUnused()
		{
			auto it = std::remove_if(m_assets.begin(), m_assets.end(), [](const reference<Asset> asset) {return asset.refCount() == 1; });
			m_assets.erase(it, m_assets.end());
		}

		bool AssetManager::hasAsset(const std::string & name) const
		{
			auto it = std::find_if(m_assets.begin(), m_assets.end(), [&](const reference<Asset>& asset) {return asset->getName() == name; });
			return it == m_assets.end();
		}

		reference<Asset> AssetManager::loadAsset(const std::string& file)
		{
			Triton::Asset* asset = nullptr;
			TR_CORE_ASSERT(
				IO::loadAssetFromDisk(file, asset, m_contextInstance).status == IO::IOStatus::IO_OK,
				"Loading of an asset failed");
			reference<Asset> res(asset);
			addAsset(res);
			return res;
		}

		void AssetManager::addAsset(reference<Asset> asset)
		{
			m_assets.push_back(asset);
		}

		reference<Asset> AssetManager::getAsset(const std::string& name) const
		{
			auto it = std::find_if(m_assets.begin(), m_assets.end(), [&](const reference<Asset>& asset) {return asset->getName() == name; });
			TR_CORE_ASSERT(it != m_assets.end(), "Trying to retrieve a asset that doesn't exist!");
			return *it;
		}
	}
}