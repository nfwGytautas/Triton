#pragma once

#ifndef TR_EDITOR // TR EDITOR

#ifndef TR_PLATFORM_WINDOWS // TR PLATFORM WINDOWS

// GL MODE

extern Triton::Application* Triton::CreateApplication(Triton::AppSettings& aSettings);
extern void Triton::Loop(Triton::Application* application);

int main(int argc, char** argv)
{
	// Initialize logger
	Triton::Log::Init();

	Triton::AppSettings appSettings;

	appSettings.argc = argc;
	appSettings.argv = argv;

	// Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication(appSettings);

	try
	{
		// Setup
		app->setup();

		Triton::Loop(app);

		// Delete the created triton application
		delete app;

		return 0;
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1; 
	}
}

#else

//#include <Windows.h>
//
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
//{
//	// Initialize logger
//	Triton::Log::Init();
//
//	Triton::AppSettings appSettings;
//
//	// Create triton application by user defined procedure
//	Triton::Application* app = Triton::CreateApplication(appSettings);
//
//	try
//	{
//		// Setup
//		app->setup();
//
//		Triton::Loop(app);
//
//		// Delete the created triton application
//		delete app;
//
//		return 0;
//	}
//	catch (...)
//	{
//		TR_ASSERT(0, "Unidentified error");
//		return 1;
//	}
//
//	return 0;
//}


int main(int argc, char** argv)
{
	// Initialize logger
	Triton::Log::Init();

	Triton::AppSettings appSettings;

	//appSettings.argc = argc;
	//appSettings.argv = argv;

	// Create triton application by user defined procedure
	Triton::Application* app = Triton::CreateApplication(appSettings);

	try
	{
		// Setup
		app->setup();

		Triton::Loop(app);

		// Delete the created triton application
		delete app;

		return 0;
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1;
	}
}


#endif // TR PLATFORM WINDOWS
#endif // TR EDITOR