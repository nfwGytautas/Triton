#include "TRpch.h"
#include "Scene.h"

#include "TritonTypes/IO2.h"

namespace Triton
{
	Scene::Scene(std::string name)
		: m_name(name), m_entities(new ECS::Registry())
	{
	}

	IO::SceneData Scene::toRawData()
	{
		IO::SceneData result;

		result.Version = "Latest";
		result.Name = this->m_name;
		result.Assets = this->m_assets;
		result.Lights = this->m_lights;
		result.Entities = this->m_entities;

		return result;
	}

	std::string Scene::getName() const
	{
		return m_name;
	}

	std::vector<std::string>& Scene::assets()
	{
		return m_assets;
	}

	Graphics::LightBuffer& Scene::lights()
	{
		return m_lights;
	}

	reference<ECS::Registry>& Scene::entities()
	{
		return m_entities;
	}

	std::vector<ECS::Entity> Scene::getByMeta(std::function<bool(const Triton::Components::MetaComponent&)> pred)
	{
		std::vector<ECS::Entity> result;

		m_entities->view<Triton::Components::MetaComponent>().each(
			[&](ECS::Entity entity, Triton::Components::MetaComponent& metaComponent) 
			{
				if (pred(metaComponent)) 
				{ 
					result.push_back(entity); 
				}
			});

		return result;
	}

	std::vector<reference<Camera>>& Scene::cameras()
	{
		return m_cameras;
	}

	reference<Camera> Scene::activeCamera() const
	{
		return m_activeCamera;
	}

	void Scene::setActiveCamera(const std::string& name)
	{
		for (auto& camera : m_cameras)
		{
			if (camera->getName() == name)
			{
				m_activeCamera = camera;
				break;
			}
		}
	}
}