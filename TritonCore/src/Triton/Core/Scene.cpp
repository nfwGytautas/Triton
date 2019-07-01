#include <TRpch.h>
#include "Scene.h"

#include "TritonPlatform/mathematical.h"
#include <glm\gtc\matrix_transform.hpp>

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::Transform, Triton::Components::Visual, Triton::Components::LightEmitter

Triton::SceneBase::SceneBase(Triton::PType::Context* context, reference<Core::InputManager> iManager)
{
	Context = context;
	Input = iManager;
}

Triton::Scene::Scene(Triton::PType::Context* context, reference<Core::InputManager> iManager)
	:SceneBase(context, iManager)
{
	Entities = std::unique_ptr<ECS::Registry>(new ECS::Registry());
	
	Camera = std::make_unique<Triton::Camera>(Vector3(0.0f, 0.0f, 0.0f));

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

void Triton::Scene::Prepare()
{
	
}

void Triton::Scene::render()
{
	
}

void Triton::Scene::update(float delta)
{
	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//shader->setUniformMatrix44("viewMatrix", view);

	model_shader->enable();

	if (Camera.get() != nullptr)
	{
		Camera->OnUpdate();

		auto viewMat = Camera->ViewMatrix();
		model_shader->setBufferValue("frame_PerFrame", "viewMatrix", &viewMat);

		model_shader->setBufferValue("CameraBuffer", "cameraPosition", &Camera->Position);

		BackgroundMaterial->Shader->setBufferValue("frame_PerFrame", "viewMatrix", &viewMat);
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

	if (Camera.get() != nullptr)
	{
		auto viewMat = Camera->ViewMatrix();
		image_shader->setBufferValue("frame_PerFrame", "viewMatrix", &viewMat);
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
