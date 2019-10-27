#include "TRpch.h"
#include "Scene.h"

#include "Triton2/Utility/Log.h"
#include "TritonTypes/IO2.h"

#include "Triton2/File/Serialize.h"
#include "Triton2/File/FormatDefinitions.h"

namespace Triton
{
	Scene::Scene()
		: m_entities(new ECS::Registry())
	{
	}

	Scene::Scene(std::string name)
		: m_name(name), m_entities(new ECS::Registry())
	{
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

	void Scene::serialize(BinaryOutputArchive& archive)
	{
		// Version
		std::string version(Serialization::c_Version_Latest);
		archive(version);

		// Name, asset names
		archive(this->m_name, this->m_assets);

		// Directional lights
		archive(this->m_lights.DirLights);

		// Point lights
		archive(this->m_lights.PointLights);

		// Spot lights
		archive(this->m_lights.SpotLights);

		// Entities
		this->m_entities->snapshot().entities(archive).component<TR_SERIALIZABLE_COMPONENTS>(archive);

		// Active camera
		archive(this->m_activeCamera->getName());

		// Cameras
		archive(this->m_cameras.size());

		for (size_t i = 0; i < m_cameras.size(); i++) 
		{
			// Serialize the type of the camera
			archive(m_cameras[i]->type());

			// Serialize the contents of the camera
			m_cameras[i]->serialize(archive);
		}
	}

	void Scene::deserialize(BinaryInputArchive& archive)
	{
		// Version
		std::string version;
		archive(version);

		// Check version
		if (version.compare(Serialization::c_Version_00_00_00) >= 0)
		{
			// Name, asset names
			archive(this->m_name, this->m_assets);

			// Directional lights
			archive(this->m_lights.DirLights);

			// Point lights
			archive(this->m_lights.PointLights);

			// Spot lights
			archive(this->m_lights.SpotLights);

			// Entities
			this->m_entities->loader().entities(archive).component<TR_SERIALIZABLE_COMPONENTS>(archive);

			// Active camera
			std::string activeCamera;
			archive(activeCamera);

			// Cameras
			size_t count = 0;
			archive(count);

			for (size_t i = 0; i < count; i++)
			{
				// Type
				std::string type;
				archive(type);

				if (type == Serialization::c_type_Static_Camera)
				{
					reference<Camera> camera = new StaticCamera();
					camera->deserialize(archive);
					m_cameras.push_back(camera);
				}
				else
				{
					TR_SYSTEM_WARN("Camera type '{0}' is not supported for version '{1}'", type, Serialization::c_Version_00_00_00);
				}
			}

			// Set active camera
			setActiveCamera(activeCamera);
		}
	}

	std::string Scene::type()
	{
		return Serialization::c_type_Scene;
	}
}