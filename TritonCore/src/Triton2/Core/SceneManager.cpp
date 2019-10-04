#include "TRpch.h"
#include "SceneManager.h"

#include <mutex>

#include "Triton2/Assert.h"
#include "TritonTypes/Asset.h"

#include "Triton2/File/File.h"
#include "Triton2/Core/ThreadPool.h"
#include "Triton2/Utility/Timer.h"

#include "Triton2/Utility/Log.h"

#include "Triton2/Core/AssetManager.h"

namespace Triton
{
	namespace Core
	{
		class SceneManager::SceneManagerImpl
		{
			const int c_threadCount = 1;
		public:
			SceneManagerImpl(AssetManager* assetManager)
				: m_assetManager(assetManager), m_tPool(c_threadCount)
			{
				TR_SYSTEM_TRACE("Created a scene manager instance with '{0}' threads", c_threadCount);
			}

			~SceneManagerImpl()
			{
				m_tPool.wait();
				shutdown();
			}

			reference<Scene> currentScene()
			{
				return m_currentScene;
			}

			void setScene(reference<Scene> scene, bool unloadPrevious)
			{
				if (unloadPrevious && m_prevScene > -1)
				{
					m_scenes.erase(m_scenes.begin() + m_prevScene);
				}
				m_currentScene = scene;
				auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const reference<Scene>& scene) { return scene == m_currentScene; });
				m_prevScene = it - m_scenes.begin();
			}

			void unloadAll()
			{
				m_scenes.clear();

				// Slightly inefficient but this method isn't going to be called frequently
				m_scenes.push_back(m_currentScene);
			}

			void shutdown()
			{
				m_scenes.clear();
			}

			bool hasScene(const std::string& name) const
			{
				auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const reference<Scene>& scene) {return scene->getName() == name; });
				return it == m_scenes.end();
			}

			void loadScene(const std::string& file)
			{
				TR_SYSTEM_DEBUG("Loading scene: '{0}'", file);

				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);

				m_tPool.run([&, file]()
				{
					auto f = file;
					loadSceneInternalMT(f); 
					TR_SYSTEM_DEBUG("Scene '{0}' loading complete", f);
				});				
			}

			void addScene(reference<Scene> scene)
			{
				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);
				m_scenes.push_back(scene);
			}

			reference<Scene> getScene(const std::string& name) const
			{
				// Create lock guard
				std::lock_guard<std::mutex> guard(m_mtx);

				auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const reference<Scene>& scene) {return scene->getName() == name; });
				TR_CORE_ASSERT(it != m_scenes.end(), "Trying to retrieve a scene that doesn't exist!");

				return *it;
			}

			void wait()
			{
				m_tPool.wait();
			}

			reference<Scene> waitFor(const std::string& name, unsigned int amount)
			{
				Utility::Timer timer(true);

				while (timer.elapsedTime() <= amount)
				{
					if (hasScene(name))
					{
						return getScene(name);
					}
				}

				return reference<Scene>(nullptr);
			}
		private:

			void loadSceneInternalMT(const std::string& file)
			{
				Triton::Scene* scene = nullptr;
				
				TR_CORE_ASSERT(
					IO::loadSceneFromDisk(file, scene).status == IO::IOStatus::IO_OK,
					"Loading of a scene failed");

				reference<Scene> res(scene);
				addScene(res);

				m_assetManager->loadAssetsByName(res->assets());
			}
		private:
			/// Thread pool used by the scene manager
			Thread::ThreadPool m_tPool;

			/// Mutex to make sure that the scene manager is thread safe
			mutable std::mutex m_mtx;

			/// Map of asset name to scene reference
			std::vector<reference<Scene>> m_scenes;

			/// The current active scene
			reference<Scene> m_currentScene;

			/// Index of the previous scene
			int m_prevScene = -1;

			/// Pointer to the asset manager instance
			AssetManager* m_assetManager;
		};

		SceneManager::SceneManager(AssetManager* assetManager)
		{
			m_impl = new SceneManagerImpl(assetManager);
		}

		SceneManager::~SceneManager()
		{
			delete m_impl;
		}

		reference<Scene> SceneManager::currentScene()
		{
			return m_impl->currentScene();
		}

		void SceneManager::setScene(reference<Scene> scene, bool unloadPrevious)
		{
			m_impl->setScene(scene, unloadPrevious);
		}

		void SceneManager::setScene(const std::string& scene, bool unloadPrevious)
		{
			setScene(m_impl->getScene(scene), unloadPrevious);
		}

		void SceneManager::unloadAll()
		{
			m_impl->unloadAll();
		}

		bool SceneManager::hasScene(const std::string& name) const
		{
			return m_impl->hasScene(name);
		}

		void SceneManager::loadScene(const std::string& file)
		{
			m_impl->loadScene(file);
		}

		void SceneManager::addScene(reference<Scene> scene)
		{
			m_impl->addScene(scene);
		}

		reference<Scene> SceneManager::getScene(const std::string& name) const
		{
			return m_impl->getScene(name);
		}

		void SceneManager::wait()
		{
			m_impl->wait();
		}

		reference<Scene> SceneManager::waitFor(const std::string& name, unsigned int amount)
		{
			return m_impl->waitFor(name, amount);
		}
	}
}