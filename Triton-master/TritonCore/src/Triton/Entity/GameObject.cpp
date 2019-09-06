#include "TRpch.h"
#include "GameObject.h"

#include "Triton/TRMacros.h"

#include "Triton/Managers/GameObjectManager.h"

namespace Triton
{
	GameObject::GameObject(relay_ptr<ECS::Registry> registry, relay_ptr<Core::GameObjectManager> manager)
		: m_registry(registry), m_manager(manager)
	{
		// Critical error since the registry should never be destroyed without
		// all corresponding game objects also being destroyed
		TR_CORE_ASSERT(m_registry.valid(), "Registry instance is nullptr");

		// Create entity id
		m_enttID = m_registry->create();
	}

	GameObject::~GameObject()
	{
		// Critical error since the registry should never be destroyed without
		// all corresponding game objects also being destroyed
		TR_CORE_ASSERT(m_registry.valid(), "Registry instance is nullptr");

		// Destroy entity with id
		m_registry->destroy(m_enttID);
	}

	void GameObject::assignTag(std::string tag)
	{
		m_tags.push_back(tag);
	}

	void GameObject::removeTag(std::string tag)
	{
		std::remove(m_tags.begin(), m_tags.end(), tag);
	}

	void GameObject::clearTags()
	{
		m_tags.clear();
	}

	bool GameObject::hasTag(std::string tag) const
	{
		return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
	}

	std::vector<std::string> GameObject::getTags() const
	{
		return m_tags;
	}

	ECS::Entity GameObject::getEntityID() const
	{
		return m_enttID;
	}

	void GameObject::setLayer(std::string layer)
	{
		m_layer = layer;
	}

	std::string GameObject::getLayer() const
	{
		return m_layer;
	}

	void GameObject::setName(std::string name)
	{
		if (name != "")
		{
			m_name = name;
		}
	}

	std::string GameObject::getName() const
	{
		return m_name;
	}

	void GameObject::attachScript(std::string scriptName)
	{
		m_manager->attachScriptToObject(scriptName, this);
	}

	void GameObject::detachScript(std::string scriptName)
	{
		m_manager->detachScriptToObject(scriptName, this);
	}
}