#pragma once

#include "TritonScript\Implementations\TRPythonModule.h"

extern Triton::Application* Triton::CreateApplication();

int main(int argc, char** argv)
{
	//Initialize logger
	Triton::Log::Init();

	//Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication();

	//Run the created application
	app->Execute();

	//Delete the created triton application
	delete app;
}