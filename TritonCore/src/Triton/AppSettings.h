#pragma once

namespace Triton {

	struct AppSettings
	{
		unsigned int WindowWidth = 600;
		unsigned int WindowHeight = 600;
		std::string WindowTitle = "Triton display";

#ifdef TR_PLATFORM_WINDOWS
		// Windows specific settings
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		PSTR pScmdline;
		int iCmdshow;
#else
		// Default to OpenGL
		int argc;
		char** argv;
#endif
	};
}