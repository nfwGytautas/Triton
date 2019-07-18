#include <TRpch.h>
#include "Scene.h"

#include "TritonPlatform/mathematical.h"
#include <glm\gtc\matrix_transform.hpp>

#include "Triton\Core\RenderBuffer.h"

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::Transform, Triton::Components::Visual, Triton::Components::LightEmitter

Triton::SceneBase::SceneBase()
{

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
	/*auto& renderBuffer = this->getClassByID((size_t)Core::TritonClasses::MainRenderBuffer).as<Triton::Core::RenderBuffer>();
	auto& Context = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();

	// Bind/enable the viewport
	//renderBuffer->addCommand<RCommands::PushViewport>(renderTo);

	// Render the background

	// Disable culling
	renderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->cullBufferState(false);
	});

	renderBuffer->addCommand<RCommands::PushMesh>(BackgroundMesh);
	renderBuffer->addCommand<RCommands::PushMaterial>(BackgroundMaterial);

	renderBuffer->addCommand<RCommands::UpdateUniformValues>
		([&](reference<PType::Shader> shader)
	{
		auto trans_mat = Triton::Core::CreateTransformationMatrix(Camera->Position, Vector3(0, 0, 0), Vector3(1, 1, 1));

		shader->setBufferValue("persistant_Persistant", "projectionMatrix", &Context->renderer->projection());
		shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);
		shader->setBufferValue("frame_PerFrame", "viewMatrix", &getViewMatrix());
	});

	// Update shader buffers and render the background
	renderBuffer->addCommand<RCommands::UpdateShaderBuffer>(PType::BufferUpdateType::ALL);
	renderBuffer->addCommand<RCommands::Draw>();

	// Enable culling
	renderBuffer->addCommand<RCommands::ChangeContextSetting>
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
			auto material = m_AssetManager->getObject(visual.Material).as<Triton::Data::Material>();

			// Push material
			renderBuffer->addCommand<RCommands::PushMaterial>(material);

			// Update uniforms
			renderBuffer->addCommand<RCommands::UpdateUniformValues>
				(
					[&](reference<PType::Shader> shader)
			{
				shader->setBufferValue("persistant_Persistant", "projectionMatrix", &Context->renderer->projection());
				shader->setBufferValue("frame_PerFrame", "viewMatrix", &getViewMatrix());
				shader->setBufferValue("CameraBuffer", "cameraPosition", &Camera->Position);

				// Push lights
				for (size_t i = 0; i < scene->m_Lights.size(); i++)
				{
					scene->m_Lights[i]->bind(shader);
				}

				shader->updateBuffers(PType::BufferUpdateType::FRAME);
				shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
			}
			);
		}

		// Get mesh object
		reference<Data::Mesh>& mesh = m_AssetManager->getObject(visual.Mesh).as<Data::Mesh>();
		reference<PType::VAO>& vao = mesh->VAO;

		if (changeMesh)
		{
			// Push mesh
			renderBuffer->addCommand<RCommands::PushMesh>(mesh);
		}

		// Update uniforms
		renderBuffer->addCommand<RCommands::UpdateUniformValues>
			(
				[&](reference<PType::Shader> shader)
		{
			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

			shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		}
		);

		// Push render command
		renderBuffer->addCommand<RCommands::Draw>();
	});

	// Change context settings
	renderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(false);
	});

	// Push image shader
	renderBuffer->addCommand<RCommands::PushShader>(image_shader);

	// Update uniforms
	renderBuffer->addCommand<RCommands::UpdateUniformValues>
		([&](reference<PType::Shader> shader)
	{
		shader->setBufferValue("persistant_Persistant", "projectionMatrix", &Context->renderer->orthographic());
		shader->setBufferValue("frame_PerFrame", "viewMatrix", &getViewMatrix());

		shader->updateBuffers(PType::BufferUpdateType::FRAME);
		shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
	});

	// Render all images in the scene
	Entities->view<Components::Transform, Components::Image>().each([&](auto& transform, auto& imageComp) {

		// Get image object
		reference<Data::Image> image = m_AssetManager->getObject(imageComp.Bitmap).as<Data::Image>();

		// Push image
		renderBuffer->addCommand<RCommands::PushImage>(image);

		// Update uniforms
		renderBuffer->addCommand<RCommands::UpdateUniformValues>
			([&](reference<PType::Shader> shader)
		{
			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

			shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		});

		// Push render command
		renderBuffer->addCommand<RCommands::Draw>();
	});

	// Change context settings
	renderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(true);
		context->renderer->default();
	});*/
}
