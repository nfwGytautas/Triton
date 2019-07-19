#include <TRpch.h>
#include "Scene.h"

#include "TritonPlatform/mathematical.h"
#include <glm\gtc\matrix_transform.hpp>

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::Transform, Triton::Components::Visual, Triton::Components::LightEmitter

Triton::SceneBase::SceneBase()
{

}

std::tuple<bool, bool> BindVisual(Triton::Components::Visual& prevVisual, Triton::Components::Visual& newVisual)
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

	return { diffMat , diffMesh };
}

Triton::Scene::Scene()
	: SceneBase()
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

void Triton::Scene::onMessage(size_t message, void* payload)
{
	switch (message)
	{
	case (size_t) Core::TritonMessageType::ClassRegistered:
	{
		m_mainRenderBuffer = this->getClassByID((size_t)Core::TritonClasses::MainRenderBuffer).as<Triton::Core::RenderBuffer>();
		m_graphicsContext = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();
		m_assetManager = this->getClassByID((size_t)Core::TritonClasses::AssetManager).as<Manager::AssetManager>();
		m_gameWindow = this->getClassByID((size_t)Core::TritonClasses::GameWindow).as<GameWindow>();

		return;
	}
	case (size_t) Core::TritonMessageType::Update:
	{
		onUpdate();
		return;
	}
	case (size_t)Core::TritonMessageType::Render:
	{
		onRender();
		return;
	}
	}
}

Triton::Matrix44& Triton::Scene::getViewMatrix() const
{
	if (Camera.get() != nullptr)
	{
		return Camera->ViewMatrix();
	}
	return Matrix44();
}

void Triton::Scene::onUpdate()
{
	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//shader->setUniformMatrix44("viewMatrix", view);

	if (Camera.get() != nullptr)
	{
		Camera->OnUpdate();
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

void Triton::Scene::onRender()
{
	// Bind/enable the viewport
	m_mainRenderBuffer->addCommand<RCommands::PushViewport>(m_gameWindow->getViewport());

	// Render the background

	// Disable culling
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->cullBufferState(false);
	});

	m_mainRenderBuffer->addCommand<RCommands::PushMesh>(BackgroundMesh);
	m_mainRenderBuffer->addCommand<RCommands::PushMaterial>(BackgroundMaterial);

	m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
		([&](reference<PType::Shader> shader)
	{
		auto trans_mat = Triton::Core::CreateTransformationMatrix(Camera->Position, Vector3(0, 0, 0), Vector3(1, 1, 1));

		shader->setBufferValue("persistant_Persistant", "projectionMatrix", &m_graphicsContext->renderer->projection());
		shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);
		shader->setBufferValue("frame_PerFrame", "viewMatrix", &getViewMatrix());
	});

	// Update shader buffers and render the background
	m_mainRenderBuffer->addCommand<RCommands::UpdateShaderBuffer>(PType::BufferUpdateType::ALL);
	m_mainRenderBuffer->addCommand<RCommands::Draw>();

	// Enable culling
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->cullBufferState(true);
	});

	// Render all entities in a scene
	m_CurrVisual.Material = -1;
	m_CurrVisual.Mesh = -1;
	Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

		// Check if visual needs to be bound
		auto[changeMat, changeMesh] = BindVisual(m_CurrVisual, visual);

		if (changeMat)
		{
			// Get material used by the entity
			auto material = m_assetManager->getAssetByID(visual.Material).as<Triton::Data::Material>();

			// Push material
			m_mainRenderBuffer->addCommand<RCommands::PushMaterial>(material);

			// Update uniforms
			m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
				(
					[&](reference<PType::Shader> shader)
			{
				shader->setBufferValue("persistant_Persistant", "projectionMatrix", &m_graphicsContext->renderer->projection());
				shader->setBufferValue("frame_PerFrame", "viewMatrix", &getViewMatrix());
				shader->setBufferValue("CameraBuffer", "cameraPosition", &Camera->Position);

				// Push lights
				for (size_t i = 0; i < m_Lights.size(); i++)
				{
					m_Lights[i]->bind(shader);
				}

				shader->updateBuffers(PType::BufferUpdateType::FRAME);
				shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
			}
			);
		}

		// Get mesh object
		reference<Data::Mesh>& mesh = m_assetManager->getAssetByID(visual.Mesh).as<Data::Mesh>();
		reference<PType::VAO>& vao = mesh->VAO;

		if (changeMesh)
		{
			// Push mesh
			m_mainRenderBuffer->addCommand<RCommands::PushMesh>(mesh);
		}

		// Update uniforms
		m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
			(
				[&](reference<PType::Shader> shader)
		{
			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

			shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		}
		);

		// Push render command
		m_mainRenderBuffer->addCommand<RCommands::Draw>();
	});

	// Change context settings
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(false);
	});

	// Push image shader
	m_mainRenderBuffer->addCommand<RCommands::PushShader>(image_shader);

	// Update uniforms
	m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
		([&](reference<PType::Shader> shader)
	{
		shader->setBufferValue("persistant_Persistant", "projectionMatrix", &m_graphicsContext->renderer->orthographic());
		shader->setBufferValue("frame_PerFrame", "viewMatrix", &getViewMatrix());

		shader->updateBuffers(PType::BufferUpdateType::FRAME);
		shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
	});

	// Render all images in the scene
	Entities->view<Components::Transform, Components::Image>().each([&](auto& transform, auto& imageComp) {

		// Get image object
		reference<Data::Image> image = m_assetManager->getAssetByID(imageComp.Bitmap).as<Data::Image>();

		// Push image
		m_mainRenderBuffer->addCommand<RCommands::PushImage>(image);

		// Update uniforms
		m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
			([&](reference<PType::Shader> shader)
		{
			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

			shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		});

		// Push render command
		m_mainRenderBuffer->addCommand<RCommands::Draw>();
	});

	// Change context settings
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(true);
		context->renderer->default();
	});
}
