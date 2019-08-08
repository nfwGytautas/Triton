#include "TRpch.h"
#include "AssetManager.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/File/File.h"
#include "TritonCache.h"
#include "GameObjectManager.h"

namespace Triton
{
	namespace Core
	{
		GameObjectManager::GameObjectManager()
		{
			m_registry = new ECS::Registry();
		}

		reference<GameObject> GameObjectManager::newObject()
		{
			GameObject* gameObject = new GameObject(m_registry.asRelay(), this);

			gameObject->setName(c_baseName + std::to_string(m_nextObjectNumber++));

			auto refToGO = reference<GameObject>(gameObject);

			m_objects.push_back(refToGO);
			m_idObjectMap[gameObject->getEntityID()] = refToGO;

			return refToGO;
		}

		void GameObjectManager::deleteObject(reference<GameObject> object)
		{
			std::remove(m_objects.begin(), m_objects.end(), object);
			m_idObjectMap.erase(object->getEntityID());
		}

		reference<GameObject> GameObjectManager::getObject(ECS::Entity id)
		{
			return m_idObjectMap[id];
		}

		void GameObjectManager::refresh()
		{
			std::vector<reference<GameObject>> objectsToDelete;

			for (auto& object : *this)
			{
				if (!m_registry->valid(object->getEntityID()))
				{
					objectsToDelete.push_back(object);
				}
			}

			for (auto& object : objectsToDelete)
			{
				deleteObject(object);
			}
		}

		std::vector<reference<GameObject>> GameObjectManager::byTag(std::string tag)
		{
			std::vector<reference<GameObject>> result;
			for (auto& obj : m_objects)
			{
				auto tags = obj->getTags();

				if (std::find(tags.begin(), tags.end(), tag) != tags.end())
				{
					result.push_back(obj);
				}
			}

			return result;
		}

		std::vector<reference<GameObject>> GameObjectManager::byName(std::string name)
		{
			std::vector<reference<GameObject>> result;
			for (auto& obj : m_objects)
			{
				if (obj->getName() == name)
				{
					result.push_back(obj);
				}
			}

			return result;
		}
	}
}