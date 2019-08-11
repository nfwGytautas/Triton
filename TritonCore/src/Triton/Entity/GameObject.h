#pragma once

#include "Triton/Entity/Registry.h"
#include "Triton/TRMacros.h"

#include "TritonTypes/relay_ptr.h"
#include "TritonTypes/reference.h"

namespace Triton
{
	namespace Core
	{
		class GameObjectManager;
	}

	// Class representing an entity in the world it acts as a wrapper
	// around the entity with additional functionality, this is also
	// the object that is passed to script functions
	class GameObject
	{
	public:
		// Empty game object constructor
		GameObject() {}

		// Game object constructor
		//	Internally creates an entity ID
		GameObject(relay_ptr<ECS::Registry> registry, relay_ptr<Core::GameObjectManager> manager);
		// Game object destructor
		//	Internally removes entity from registry
		virtual ~GameObject();

		// Get component of type T from the game object
		// Returns the reference to the component
		template<typename T>
		T& getComponent()
		{
			// Critical error since the registry should never be destroyed without
			// all corresponding game objects also being destroyed
			TR_CORE_ASSERT(m_registry.valid(), "Registry instance is nullptr");

			return m_registry->get<T>(m_enttID);
		}

		// Add component of type T for the game object (default initialized)
		// Returns the reference to the component
		template<typename T>
		T& addComponent()
		{
			// Critical error since the registry should never be destroyed without
			// all corresponding game objects also being destroyed
			TR_CORE_ASSERT(m_registry.valid(), "Registry instance is nullptr");

			return m_registry->assign<T>(m_enttID);
		}

		// Add component of type T for the game object (argument initialized)
		// Returns the reference to the component
		template<typename T, typename... Args>
		T& addComponent(Args&& ... args)
		{
			// Critical error since the registry should never be destroyed without
			// all corresponding game objects also being destroyed
			TR_CORE_ASSERT(m_registry.valid(), "Registry instance is nullptr");

			return m_registry->assign<T>(m_enttID, std::forward<Args>(args)...);
		}

		// Check if object contains a component
		template<typename T>
		bool hasComponent()
		{
			return m_registry->has<T>(m_enttID);
		}

		// Assigns a tag to the game object
		void assignTag(std::string tag);

		// Removes a tag from the game object
		void removeTag(std::string tag);

		// Clears all tags
		void clearTags();

		// Check if game object has tag
		bool hasTag(std::string tag) const;

		// Return the tag vector
		std::vector<std::string> getTags() const;
		
		// Returns the internal entity ID
		ECS::Entity getEntityID() const;

		// Sets the layer
		void setLayer(std::string layer);

		// Get the object layer
		std::string getLayer() const;

		// Set the object's name
		void setName(std::string name);

		// Get the object's name
		std::string getName() const;

		// Attach a script to this game object
		void attachScript(std::string scriptName);

		// Detach a script from this game object
		void detachScript(std::string scriptName);
	private:
		// ENTT Registry instance that is used by the object
		relay_ptr<ECS::Registry> m_registry;

		// Manager instance
		relay_ptr<Core::GameObjectManager> m_manager;

		// The internal id that is used for getting components from the registry
		ECS::Entity m_enttID;

		// Vector containing all the tags for a game object
		std::vector<std::string> m_tags;

		// The layer of the game object
		//	Triton allows ordering scenes with layers game objects also 
		//	have their own layers since a scene can have a terrain so you will want to 
		//	render the terrain then the layer
		std::string m_layer;

		// The name of the game object
		std::string m_name;
	};
}