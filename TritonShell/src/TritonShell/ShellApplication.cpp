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

		prtc_BatchSystem = std::make_unique<Triton::Systems::BatchSystem>();

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

		prtc_BatchSystem->OnUpdate(*prtc_EntityRegistry.get(), 0.0f);

		#ifndef TR_DISABLE_SCRIPTING 
			TR_PYTHON_SCRIPT_GUARD(prtc_py_Update.attr("entry").call(prtc_EntityRegistry.get(), prtc_Delta));
		#endif
	}

	void ShellApplication::RestartShell()
	{
		prtc_EntityRegistry->reset();

		#ifndef TR_DISABLE_SCRIPTING
			py_ReloadModules();
			TR_PYTHON_SCRIPT_GUARD(this->prtc_py_PreExecution.attr("entry").call(prtc_EntityRegistry.get()));
		#endif

		PreExecutionSetup();
	}

	void ShellApplication::Execute()
	{
		UpdateProjectionMatrix();

		PreExecutionSetup();

		#ifndef TR_DISABLE_SCRIPTING 
			TR_PYTHON_SCRIPT_GUARD(this->prtc_py_PreExecution.attr("entry").call(prtc_EntityRegistry.get()));
		#endif

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
}