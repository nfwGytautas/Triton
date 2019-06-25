#include <TRpch.h>
#include "Scene.h"

#include "TritonPlatform/mathematical.h"
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

		//prevVisual = newVisual;

		return { diffMat , diffMesh};
	}

	SceneBase::SceneBase(Triton::PType::Context* context)
	{
		Context = context;
	}
}

Triton::Scene::Scene(Triton::PType::Context* context)
	:SceneBase(context)
{
	Entities = std::unique_ptr<ECS::Registry>(new ECS::Registry());
	
	m_Camera = std::make_unique<Camera>(Vector3(0.0f, 0.0f, 0.0f));

	//prtc_Shader = std::shared_ptr<Core::Shader>(Core::Shader::Create(
	//	Core::ShaderSettings(
	//		"D:/Programming/Test files/nfw/shaders/triton/v4.shader",
	//		"D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader")));
}

Triton::Scene::~Scene()
{
}

void Triton::Scene::addLight(std::string type, reference<Graphics::Light> light)
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

void Triton::Scene::addAsset(size_t id, reference<Resource::Asset> asset)
{
	m_Assets[id] = asset;
}

void Triton::Scene::Prepare()
{
	
}

void Triton::Scene::render()
{
	float fov = 3.141592654f / 4.0f;
	auto proj_mat = Triton::Core::CreateProjectionMatrix(923, 704, fov, 0.1f, 100.0f);
	model_shader->setBufferValue("Persistant", "projectionMatrix", &proj_mat);

	auto ortho_mat = Triton::Core::CreateOrthographicMatrix(923, 704, 0.1f, 100.0f);
	image_shader->setBufferValue("Persistant", "projectionMatrix", &ortho_mat);

	model_shader->enable();

	model_shader->updateBuffers(PType::BufferUpdateType::FRAME);

	model_shader->updateBuffers(Triton::PType::BufferUpdateType::PERSISTANT);

	Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

		auto[changeMat, changeMesh] = BindVisual(m_CurrVisual, visual);

		if (changeMat)
		{
			m_Assets[visual.Material].as<Data::Material>()->enable();
		}

		reference<Data::Mesh> mesh = m_Assets[visual.Mesh].as<Data::Mesh>();

		if (changeMesh)
		{
			mesh->enable();
		}

		auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
		model_shader->setBufferValue("PerObject", "transformationMatrix", &trans_mat);

		model_shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		//shader->updateBuffers(PType::BufferUpdateType::ALL);
		Context->renderer->render(mesh->object().as<PType::Renderable>());
	});

	Context->depthBufferState(false);
	
	image_shader->enable();
	
	image_shader->updateBuffers(PType::BufferUpdateType::FRAME);

	image_shader->updateBuffers(Triton::PType::BufferUpdateType::PERSISTANT);

	Entities->view<Components::Transform, Components::Image>().each([&](auto& transform, auto& imageComp) {
	
		reference<Data::Image> image = m_Assets[imageComp.Bitmap].as<Data::Image>();
	
		image->enable();
	
		auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
		image_shader->setBufferValue("PerObject", "transformationMatrix", &trans_mat);
	
		image_shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		Context->renderer->render(image->object().as<PType::Renderable>());
	});
	
	Context->depthBufferState(true);
}

void Triton::Scene::update(float delta)
{
	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//shader->setUniformMatrix44("viewMatrix", view);

	model_shader->enable();

	if (m_Camera.get() != nullptr)
	{
		m_Camera->OnUpdate();

		auto viewMat = m_Camera->ViewMatrix();
		model_shader->setBufferValue("PerFrame", "viewMatrix", &viewMat);

		model_shader->setBufferValue("CameraBuffer", "cameraPosition", &m_Camera->Position);

	}

	for (unsigned int i = 0; i < m_Lights.size(); i++)
	{
		m_Lights[i]->bind(model_shader);
	}

	for (auto pair : m_LightCounts)
	{
		//shader->setBufferValue("num_of_" + pair.first + "s", pair.second + 1);
	}

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
	

	image_shader->enable();

	if (m_Camera.get() != nullptr)
	{
		auto viewMat = m_Camera->ViewMatrix();
		image_shader->setBufferValue("PerFrame", "viewMatrix", &viewMat);
	}

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

void Triton::Scene::UpdateOrthographic(Triton::Matrix44 aNewOrthographic)
{
}
