#pragma once

#ifndef TR_DISABLE_GUI
	#include "imgui.h"
#endif

extern Triton::Application* Triton::CreateApplication(Triton::AppSettings& aSettings);

int main(int argc, char** argv)
{
	//Initialize logger
	Triton::Log::Init();

	Triton::AppSettings appSettings;

	//Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication(appSettings);

	try
	{
		//Run the created application
		app->Execute();

		//Delete the created triton application
		delete app;

		return 0;
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1;
	}
}