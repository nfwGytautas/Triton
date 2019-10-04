#include "TRpch.h"
#include "AssetManager.h"

#include <mutex>

#include "Triton2/Assert.h"
#include "TritonTypes/Asset.h"

#include "Triton2/File/File.h"
#include "Triton2/Core/ThreadPool.h"
#include "Triton2/Utility/Timer.h"

#include "Triton2/Utility/Log.h"

#include "TritonPlatform2/CrossTypes/Core/Context.h"
#include "TritonTypes/AssetDictionary.h"

namespace Triton
{
	namespace Core
	{
		class AssetManager::AssetManagerImpl
		{
			const int c_threadCount = 2;
		public:
			AssetManagerImpl(Graphics::Context* context)
				: m_contextInstance(context), m_tPool(c_threadCount)
			{
				TR_SYSTEM_TRACE("Created an asset manager instance with '{0}' threads", c_threadCount);
			}

			~AssetManagerImpl()
			{
				m_tPool.wait();
				unloadAll();
			}

			void unloadAssets(const std::vector<std::string>& assets)
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

			void unloadAll()
			{
				m_assets.clear();
			}

			void unloadExcept(const std::vector<std::string>& assets)
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

			void unloadUnused()
			{
				auto it = std::remove_if(m_assets.begin(), m_assets.end(), [](const reference<Asset> asset) {return asset.refCount() == 1; });
				m_assets.erase(it, m_assets.end());
			}

			void unloadDictionaries()
			{
				m_dictionaries.clear();
			}

			bool hasAsset(const std::string& name) const
			{
				auto it = std::find_if(m_assets.begin(), m_assets.end(), [&](const reference<Asset>& asset) {return asset->getName() == name; });
				return it == m_assets.end();
			}

			void loadAssetByName(const std::string& name)
			{
				TR_SYSTEM_DEBUG("Loading asset by name: '{0}'", name);

				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);

				m_tPool.run([&, name]()
				{
					auto f = name;
					loadAssetByNameInternalMT(f);
					TR_SYSTEM_DEBUG("Asset '{0}' loading complete", f);
				});
			}

			void loadAsset(const std::string& file)
			{
				TR_SYSTEM_DEBUG("Loading asset: '{0}'", file);

				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);

				m_tPool.run([&, file]()
				{
					auto f = file;
					loadAssetInternalMT(f); 
					TR_SYSTEM_DEBUG("Asset '{0}' loading complete", f);
				});
			}

			void loadDictionary(const std::string& dict)
			{
				TR_SYSTEM_DEBUG("Loading dictionary: '{0}'", dict);

				AssetDictionary* dictionary = AssetDictionary::loadFromFile(dict);
				m_dictionaries.push_back(dictionary);

				TR_SYSTEM_DEBUG("Asset manager filled with '{0}' new associations", dictionary->size());
			}

			void addAsset(reference<Asset> asset)
			{
				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);
				m_assets.push_back(asset);
			}

			void addDictionary(reference<AssetDictionary> dictionary)
			{
				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);
				m_dictionaries.push_back(dictionary);
			}

			reference<Asset> getAsset(const std::string& name) const
			{
				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);

				auto it = std::find_if(m_assets.begin(), m_assets.end(), [&](const reference<Asset>& asset) {return asset->getName() == name; });
				TR_CORE_ASSERT(it != m_assets.end(), "Trying to retrieve an asset that doesn't exist!");

				if (!(*it)->isCreated())
				{
					(*it)->create(m_contextInstance);
				}

				return *it;
			}

			void wait()
			{
				m_tPool.wait();
			}

			reference<Asset> waitFor(const std::string& name, unsigned int amount)
			{
				Utility::Timer timer(true);

				while (timer.elapsedTime() <= amount)
				{
					if (hasAsset(name))
					{
						return getAsset(name);
					}
				}

				return reference<Asset>(nullptr);
			}
		private:

			void loadAssetInternalMT(const std::string& file)
			{
				Triton::Asset* asset = nullptr;
				
				TR_CORE_ASSERT(
					IO::loadAssetFromDisk(file, asset).status == IO::IOStatus::IO_OK,
					"Loading of an asset failed");

				reference<Asset> res(asset);
				addAsset(res);
			}

			void loadAssetByNameInternalMT(const std::string& name)
			{
				Triton::Asset* asset = nullptr;

				// Find the asset inside a dictionary and then load it
				bool found = false;
				for (auto i = m_dictionaries.rbegin(); i != m_dictionaries.rend(); ++i) 
				{
					if ((*i)->has(name))
					{
						std::string path = (*i)->getData(name).Path;

						TR_CORE_ASSERT(
							IO::loadAssetFromDisk(path, asset).status == IO::IOStatus::IO_OK,
							"Loading of an asset failed");

						found = true;
						break;
					}
				}

				TR_CORE_ASSERT(
					found, "Asset with specified name cannot be found inside any loaded dictionaries");

				reference<Asset> res(asset);
				addAsset(res);
			}
		private:
			/// Thread pool used by the asset manager
			Thread::ThreadPool m_tPool;

			/// Mutex to make sure that the asset manager is thread safe
			mutable std::mutex m_mtx;

			/// Map of asset name to asset reference
			std::vector<reference<Asset>> m_assets;

			/// Pointer to the context instance
			Graphics::Context* m_contextInstance;

			/// Dictionaries that are loaded into the asset manager
			std::vector<reference<AssetDictionary>> m_dictionaries;
		};

		AssetManager::AssetManager(Graphics::Context* context)
		{
			m_impl = new AssetManagerImpl(context);
		}

		AssetManager::~AssetManager()
		{
			delete m_impl;
		}

		void AssetManager::unloadAssets(const std::vector<std::string>& assets)
		{
			m_impl->unloadAssets(assets);
		}

		void AssetManager::unloadAll()
		{
			m_impl->unloadAll();
		}

		void AssetManager::unloadExcept(const std::vector<std::string>& assets)
		{
			m_impl->unloadExcept(assets);
		}

		void AssetManager::unloadUnused()
		{
			m_impl->unloadUnused();
		}

		bool AssetManager::hasAsset(const std::string& name) const
		{
			return m_impl->hasAsset(name);
		}

		void AssetManager::loadDictionary(const std::string& dict)
		{
			m_impl->loadDictionary(dict);
		}

		void AssetManager::unloadDictionaries()
		{
			m_impl->unloadDictionaries();
		}

		void AssetManager::loadAssetByName(const std::string& name)
		{
			m_impl->loadAssetByName(name);
		}

		void AssetManager::loadAssetsByName(const std::vector<std::string>& names)
		{
			for (const std::string& name : names)
			{
				loadAssetByName(name);
			}
		}

		void AssetManager::loadAsset(const std::string& file)
		{
			m_impl->loadAsset(file);
		}

		void AssetManager::loadAssets(const std::vector<std::string>& files)
		{
			for (const std::string& file : files)
			{
				loadAsset(file);
			}
		}

		void AssetManager::addAsset(reference<Asset> asset)
		{
			m_impl->addAsset(asset);
		}

		void AssetManager::addDictionary(reference<AssetDictionary> dictionary)
		{
			m_impl->addDictionary(dictionary);
		}

		reference<Asset> AssetManager::getAsset(const std::string& name) const
		{
			return m_impl->getAsset(name);
		}

		void AssetManager::wait()
		{
			m_impl->wait();
		}

		reference<Asset> AssetManager::waitFor(const std::string& name, unsigned int amount)
		{
			return m_impl->waitFor(name, amount);
		}
	}
}