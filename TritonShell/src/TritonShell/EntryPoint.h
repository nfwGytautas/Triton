#pragma once

#ifndef TR_DISABLE_GUI
	#include "imgui.h"
#endif

extern Triton::Application* Triton::CreateApplication(Triton::AppSettings& aSettings);

int main(int argc, char** argv)
{
#ifndef TR_DISABLE_GUI
	//Init imgui
	ImGuiContext* imguiContext = ImGui::CreateContext();
#endif

	//Initialize logger
	Triton::Log::Init();

	Triton::AppSettings appSettings;

#ifndef TR_DISABLE_GUI
	appSettings.ImGUIContext = imguiContext;
#endif

	//Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication(appSettings);


	try
	{
		//Run the created application
		app->Execute();

		//Delete the created triton application
		delete app;

#ifndef TR_DISABLE_GUI
		ImGui::DestroyContext(imguiContext);
#endif

		return 0;
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1;
	}
}