#pragma once

extern Triton::Application* Triton::CreateApplication();

int main(int argc, char** argv)
{
	//Initialize logger
	Triton::Log::Init();

	//Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication();

	//Run the created application
	app->Run();

	//Delete the created triton application
	delete app;
}