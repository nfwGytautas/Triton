#include <TRpch.h>
#include "Scene.h"

#include "TritonTypes/mathematical.h"
#include <glm\gtc\matrix_transform.hpp>

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::Transform, Triton::Components::Visual, Triton::Components::LightEmitter

namespace Triton
{
	std::tuple<bool, bool> BindVisual(Components::Visual& prevVisual, Components::Visual& newVisual)
	{
		bool diffMat = false;
		bool diffMesh = false;

		if (prevVisual.Material != newVisual.Material)
		{
			diffMat = true;
		}

		if (prevVisual.Mesh != newVisual.Mesh)
		{
			diffMesh = true;
		}

		prevVisual = newVisual;

		return { diffMat , diffMesh};
	}
}

Triton::Scene::Scene(relay_ptr<Triton::PType::Context> context)
{
	Entities = std::unique_ptr<ECS::Registry>(new ECS::Registry());
	
	m_Camera = std::make_unique<Camera>(Vector3(0.0f, 0.0f, 0.0f));
	m_context = context;

	//prtc_Shader = std::shared_ptr<Core::Shader>(Core::Shader::Create(
	//	Core::ShaderSettings(
	//		"D:/Programming/Test files/nfw/shaders/triton/v4.shader",
	//		"D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader")));
}

Triton::Scene::~Scene()
{
	for (auto pair : m_Assets)
	{
		pair.second->destroyAsset(m_context, nullptr);
	}

	for (unsigned int i = 0; i < m_Lights.size(); i++)
	{
		delete m_Lights[i];
	}
}

void Triton::Scene::addLight(std::string type, Triton::Graphics::Light* light)
{
	m_Lights.push_back(light);

	if (m_LightCounts.find(type) == m_LightCounts.end())
	{
		m_LightCounts[type] = 0;
		light->Slot = 0;
	}
	else
	{
		unsigned int slot = m_LightCounts[type];
		slot++;
		light->Slot = slot;
	}
}

void Triton::Scene::addAsset(size_t id, Triton::Resource::Asset* asset)
{
	m_Assets[id] = asset;
}

void Triton::Scene::Prepare()
{
	
}

void Triton::Scene::render()
{
	shader->enable();

	Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

		auto[changeMat, changeMesh] = BindVisual(m_CurrVisual, visual);

		if (changeMat)
		{
			static_cast<Data::Material*>(m_Assets[visual.Material])->enable();
		}

		if (changeMesh)
		{
			static_cast<Data::Mesh*>(m_Assets[visual.Mesh])->enable();
		}

		auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
		shader->setUniformMatrix44("transformationMatrix", trans_mat);

		m_context->renderer->render(static_cast<Data::Mesh*>(m_Assets[visual.Mesh])->object());
	});

	shader->disable();
}

void Triton::Scene::Update(float aDelta)
{
	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//shader->setUniformMatrix44("viewMatrix", view);

	shader->enable();

	if (m_Camera.get() != nullptr)
	{
		m_Camera->OnUpdate();
		shader->setUniformMatrix44("viewMatrix", m_Camera->ViewMatrix());
	}

	for (unsigned int i = 0; i < m_Lights.size(); i++)
	{
		m_Lights[i]->bind(shader);
	}

	for (auto pair : m_LightCounts)
	{
		shader->setUniformInt("num_of_" + pair.first + "s", pair.second + 1);
	}

	shader->disable();

	Entities->sort<Components::Visual>([&](const auto &lhs, const auto &rhs) {
	
		if (lhs.Mesh < rhs.Mesh)
		{
			return true;
		}
		else if (lhs.Mesh > rhs.Mesh)
		{
			return false;
		}
	
		if (lhs.Material < rhs.Material)
		{
			return true;
		}
		else if (lhs.Material > rhs.Material)
		{
			return false;
		}
	
		return false;
	});
	

	//m_Materials.ForEach([&] (auto& obj)
	//{
	//	auto shader = obj->Shader();
	//
	//	shader->Enable();
	//
	//	if (m_Camera.get() != nullptr)
	//	{
	//		m_Camera->OnUpdate();
	//		shader->SetUniform("viewMatrix", m_Camera->ViewMatrix());
	//	}
	//
	//	glm::mat4 view = glm::mat4(1.0f);
	//	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//	shader->SetUniform("viewMatrix", view);
	//
	//	shader->Disable();
	//});

}

void Triton::Scene::destroy()
{
}

void Triton::Scene::UpdateProjection(Triton::Matrix44 aNewProjection)
{
	/*m_Shaders.ForEach(
		[&](relay_ptr<Core::Shader> shader) 
		{
			shader->Enable();
			shader->SetUniform("projectionMatrix", aNewProjection);
			shader->Disable();
		});*/
}
