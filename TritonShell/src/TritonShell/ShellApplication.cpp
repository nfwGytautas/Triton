#include "ShellApplication.h"

#include "Triton\Logger\Log.h"
#include "Triton\Core\Math\Math.h"
#include <glm\gtc\matrix_transform.hpp>

#include <cereal\archives\json.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\string.hpp>

namespace Triton {

	ShellApplication::ShellApplication(const Triton::AppSettings& aSettings)
		: Application(aSettings), EventListener(prtc_EventManager.get())
	{
		prtc_EntityRegistry = std::shared_ptr<ECS::Registry>(new ECS::Registry());

		prtc_Camera = std::make_shared<Camera>(Vector3(0.0f, 0.0f, 0.0f));

		#ifndef TR_DISABLE_EDITOR_TOOLS
			prtc_Console = std::make_shared<Tools::GUIConsole>();
			prtc_Console->AddCommand("RESTART_SHELL", std::bind(&ShellApplication::RestartShell, this));

			prtc_ComponentVisualizer = std::make_shared<Tools::GUIComponentVisualizer>(prtc_EntityRegistry);
			prtc_Console->AddCommand("ENTITY_VISUALIZER", [&]() { prtc_ComponentVisualizer->Enable(); prtc_ComponentVisualizer->IsOpen = true; });		

			prtc_Console->AddCommand("SAVE", [&]() { 
				std::ofstream os("out.cereal", std::ios::binary);
				cereal::BinaryOutputArchive archive(os);

				//archive(*prtc_DataMap.get());

				//prtc_EntityRegistry->snapshot().entities(archive).destroyed(archive)
				//	.component<Components::Transform>(archive);
			});

			prtc_Console->AddCommand("LOAD", [&]() {
				std::ifstream is("out.cereal", std::ios::binary);
				cereal::BinaryInputArchive archive(is);

				//archive(*prtc_DataMap.get());

				//prtc_EntityRegistry->loader().entities(archive).destroyed(archive)
				//	.component<Components::Transform>(archive);
			});
		#endif
	}
	
	ShellApplication::~ShellApplication()
	{
	}

	void ShellApplication::RunShell()
	{	
		prtc_Shader->Enable();

		if(prtc_Camera.get() != nullptr)
		{
			prtc_Camera->OnUpdate();
			prtc_Shader->SetUniform("viewMatrix", prtc_Camera->ViewMatrix());
		}

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		prtc_Shader->SetUniform("viewMatrix", view);

		prtc_Shader->Disable();
	}

	void ShellApplication::RestartShell()
	{
		prtc_EntityRegistry->reset();

		PreExecutionSetup();
	}

	void ShellApplication::Execute()
	{
		UpdateProjectionMatrix();

		PreExecutionSetup();

		#ifndef TR_DISABLE_EDITOR_TOOLS
			prtc_GUIS->AddGUI(prtc_Console);
			prtc_GUIS->AddGUI(prtc_ComponentVisualizer);
		#endif

		while (!prtc_Display->Closed())
		{
			RunShell();
			Run();
		}
	}

	void ShellApplication::Render()
	{
		prtc_EntityRegistry->sort<Components::Visual>([&](const auto &lhs, const auto &rhs) {

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
		
		prtc_EntityRegistry->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

			BindVisual(visual);

			prtc_Renderer->AddAction<RenderActions::ChangeShaderUniform>(
				std::make_shared<ShaderUniforms::Matrix44Uniform>(
					"transformationMatrix",
					Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale)
					));

			prtc_Renderer->AddAction<RenderActions::Render>();
		});
	}

	void ShellApplication::BindVisual(Components::Visual& aVisual)
	{
		if (m_CurrentVisual.Material != aVisual.Material)
		{
			std::shared_ptr<Data::Material> material = prtc_Materials.Take(aVisual.Material);

			prtc_Renderer->AddAction<RenderActions::ChangeMaterial>(material);
			prtc_Renderer->AddAction<RenderActions::BindShader>(material->Shader());

			prtc_EntityRegistry->view<Components::LightEmitter>().each([&](auto& emitter) {

				prtc_Renderer->AddAction<RenderActions::BindLight>(prtc_Lights.Take(emitter.Light));
			});

			prtc_Renderer->AddAction<RenderActions::ChangeShaderUniform>(
				std::make_shared<ShaderUniforms::Vector3Uniform>(
					"camera.position",
					prtc_Camera->Position
					));
		}

		if (m_CurrentVisual.Mesh != aVisual.Mesh)
		{
			std::shared_ptr<Data::Mesh> mesh = prtc_Meshes.Take(aVisual.Mesh);
			
			prtc_Renderer->AddAction<RenderActions::ChangeMesh>(mesh);
		}

		m_CurrentVisual = aVisual;
	}
}