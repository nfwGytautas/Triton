#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ContextManager
			{
			public:
				TRITON_API static TRITON_WINDOW_CONTEXT GetContext();
				TRITON_API static void SetContext(TRITON_WINDOW_CONTEXT aContext);
			private:
				TRITON_API static TRITON_WINDOW_CONTEXT Context;
			};

			TRITON_API bool Initialize();
			TRITON_API bool CreateContext();

			TRITON_API void ChangeViewport(double Width, double Height);
		}
	}
}