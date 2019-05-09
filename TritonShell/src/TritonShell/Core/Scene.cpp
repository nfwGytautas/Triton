#include "Scene.h"

#include "Triton\Core\Renderer\RenderAction.h"
#include "Triton\Core\Renderer\Actions\RenderActions.h"
#include "Triton\Core\Math\Math.h"
#include <glm\gtc\matrix_transform.hpp>

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::Transform, Triton::Components::Visual, Triton::Components::LightEmitter

Triton::Scene::Scene()
{
	m_Registry = std::unique_ptr<ECS::Registry>(new ECS::Registry());

	m_Camera = std::make_unique<Camera>(Vector3(0.0f, 0.0f, 0.0f));

	//prtc_Shader = std::shared_ptr<Core::Shader>(Core::Shader::Create(
	//	Core::ShaderSettings(
	//		"D:/Programming/Test files/nfw/shaders/triton/v4.shader",
	//		"D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader")));
}

Triton::Scene::~Scene()
{
	
}

void Triton::Scene::Prepare()
{
	
}

void Triton::Scene::Render(relay_ptr<Core::Renderer> aRenderer)
{
	m_Registry->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

		BindVisual(visual, aRenderer);

		aRenderer->AddAction<RenderActions::ChangeShaderUniform>(
			std::make_shared<ShaderUniforms::Matrix44Uniform>(
				"transformationMatrix",
				Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale)
				));

		aRenderer->AddAction<RenderActions::Render>();
	});
}

void Triton::Scene::Update(float aDelta)
{
	m_Registry->sort<Components::Visual>([&](const auto &lhs, const auto &rhs) {

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


	m_Materials.ForEach([&] (auto& obj)
	{
		auto shader = obj->Shader();

		shader->Enable();

		if (m_Camera.get() != nullptr)
		{
			m_Camera->OnUpdate();
			shader->SetUniform("viewMatrix", m_Camera->ViewMatrix());
		}

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		shader->SetUniform("viewMatrix", view);

		shader->Disable();
	});

}

void Triton::Scene::UpdateProjection(Triton::Matrix44 aNewProjection)
{
	m_Shaders.ForEach(
		[&](relay_ptr<Core::Shader> shader) 
		{
			shader->Enable();
			shader->SetUniform("projectionMatrix", aNewProjection);
			shader->Disable();
		});
}

void Triton::Scene::BindVisual(Components::Visual & aVisual, relay_ptr<Core::Renderer> aRenderer)
{
	if (m_CurrentVisual.Material != aVisual.Material)
	{
		relay_ptr<Data::Material> material = m_Materials.Take(aVisual.Material);

		aRenderer->AddAction<RenderActions::ChangeMaterial>(material);
		aRenderer->AddAction<RenderActions::BindShader>(material->Shader());

		aRenderer->AddAction<RenderActions::ChangeShaderUniform>(
			std::make_shared<ShaderUniforms::Vector3Uniform>(
				"camera.position",
				m_Camera->Position
				));

		Singleton::State::GetInstance()->ResetLights();

		m_Registry->view<Components::LightEmitter>().each([&](auto& emitter) {

			aRenderer->AddAction<RenderActions::BindLight>(m_Lights.Take(emitter.Light));
		});

		aRenderer->AddAction<RenderActions::UpdateUniforms>();
	}

	if (m_CurrentVisual.Mesh != aVisual.Mesh)
	{
		relay_ptr<Data::Mesh> mesh = m_Meshes.Take(aVisual.Mesh);

		aRenderer->AddAction<RenderActions::ChangeMesh>(mesh);
	}

	m_CurrentVisual = aVisual;
}
