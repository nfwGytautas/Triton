#pragma once

extern Triton::Application* Triton::CreateApplication();

int main(int argc, char** argv)
{
	//Create triton application by user defined procedure
	auto app = Triton::CreateApplication();
	//Output app information
	app->PrintInformation();
	//Run the created application
	app->Run();
	//Delete the created triton application
	delete app;
}