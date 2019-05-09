#include "ShellApplication.h"

#include "Triton\Logger\Log.h"
#include "Triton\Core\Math\Math.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Triton {

	ShellApplication::ShellApplication(const Triton::AppSettings& aSettings)
		: Application(aSettings), EventListener(prtc_EventManager.get())
	{
		prtc_Scene = std::unique_ptr<Scene>(new Scene());

		#ifndef TR_DISABLE_EDITOR_TOOLS
			prtc_Console = std::make_shared<Tools::GUIConsole>();
			prtc_Console->AddCommand("RESTART_SHELL", std::bind(&ShellApplication::RestartShell, this));

			prtc_ComponentVisualizer = std::make_shared<Tools::GUIComponentVisualizer>(prtc_Scene->Registry());
			prtc_Console->AddCommand("ENTITY_VISUALIZER", [&]() { prtc_ComponentVisualizer->Enable(); prtc_ComponentVisualizer->IsOpen = true; });		

			prtc_Console->AddCommand("SAVE", [&]() { 
				std::ofstream os("test.scene", std::ios::binary);
				cereal::BinaryOutputArchive archive(os);

				archive(*prtc_Scene.get());
			});

			prtc_Console->AddCommand("LOAD", [&]() {
				std::ifstream is("test.scene", std::ios::binary);
				cereal::BinaryInputArchive archive(is);

				archive(*prtc_Scene.get());

				prtc_Scene->UpdateProjection(this->GetProjectionMatrix());
				prtc_Scene->Prepare();
			});
		#endif
	}
	
	ShellApplication::~ShellApplication()
	{
	}

	void ShellApplication::RunShell()
	{	
		prtc_Scene->Update(prtc_Delta);
	}

	void ShellApplication::RestartShell()
	{
		PreExecutionSetup();
	}

	void ShellApplication::Execute()
	{
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
		prtc_Scene->Render(prtc_Renderer);
	}

}