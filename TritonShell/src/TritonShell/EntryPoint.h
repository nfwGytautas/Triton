#pragma once

#ifndef TR_DISABLE_SCRIPTING 
	#include "TritonScript\Implementations\TRPythonModule.h"
#endif

extern Triton::Application* Triton::CreateApplication();

int main(int argc, char** argv)
{
#ifndef TR_DISABLE_SCRIPTING 
	Triton::Scripting::TRPythonScriptingInterface::Init();
#endif

	//Initialize logger
	Triton::Log::Init();

	//Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication();

	try
	{
		//Run the created application
		app->Execute();

		//Delete the created triton application
		delete app;

#ifndef TR_DISABLE_SCRIPTING 
		Triton::Scripting::TRPythonScriptingInterface::Disable();
#endif

		return 0;
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1;
	}
}