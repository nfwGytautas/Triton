#pragma once

#include "Triton/Core/TritonClass.h"

#include "Triton/Entity/Components/TritonComponents.h"
#include "Triton/Entity/Registry.h"

#include "Triton/Managers/ManagerBase.h"
#include "Triton/Entity/GameObject.h"

#include "TritonTypes/reference.h"

namespace Triton
{
	namespace Core
	{
		class GameObjectManager
		{
			const std::string c_baseName = "gObject_";
		public:
			GameObjectManager();
			virtual ~GameObjectManager() {};

			// Create a new game object
			reference<GameObject> newObject();

			// Deletes a game object from the manager
			void deleteObject(reference<GameObject> object);

			// Gets the game object with specified registry id
			reference<GameObject> getObject(ECS::Entity id);

			// Iterators for traversing all game objects
			std::vector<reference<GameObject>>::iterator begin() { return m_objects.begin(); }
			std::vector<reference<GameObject>>::iterator end() { return m_objects.end(); }
			std::vector<reference<GameObject>>::const_iterator begin() const { return m_objects.begin(); }
			std::vector<reference<GameObject>>::const_iterator end() const { return m_objects.end(); }

			// Get grouped list of components and entities
			template<typename... Components>
			entt::view<Components...> getEntityGroup()
			{
				return m_registry->view<Components...>();
			}

			// Clears all invalid entities
			void refresh();

			// Returns all game objects with specified tag
			std::vector<reference<GameObject>> byTag(std::string tag);

			// Returns all game objects with specified name
			std::vector<reference<GameObject>> byName(std::string name);
		private:
			// The registry for this manager
			reference<ECS::Registry> m_registry;

			// The game objects of this manager
			std::vector<reference<GameObject>> m_objects;

			// The map for the game object and entity id
			std::unordered_map<ECS::Entity, reference<GameObject>> m_idObjectMap;

			// The next object id for creating non repeating names
			unsigned int m_nextObjectNumber = 0;
		};
	}
}