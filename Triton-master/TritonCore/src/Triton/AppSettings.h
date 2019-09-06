#pragma once

#include <string>

namespace Triton {

	struct AppSettings
	{
		unsigned int WindowWidth = 600;
		unsigned int WindowHeight = 600;
		std::string WindowTitle = "Triton display";

		bool ShouldCreateWindow = true;

		// If Triton shouldn't create it's own window then the HWND should
		// be supplied
		void* HWND;
	};
}