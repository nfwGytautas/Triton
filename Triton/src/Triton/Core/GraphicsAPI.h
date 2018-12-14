#pragma once
#include "TRpch.h"

#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			enum class ShaderType
			{
				Vertex,
				Fragment,
			};

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
			TRITON_API void ContextLink(void* aLink);

			TRITON_API void ChangeViewport(double aWidth, double aHeight);

			TRITON_API void PollCallbacks();
			TRITON_API void SetupContextCallbacks();

			TRITON_API unsigned int CreateShader(ShaderType aShaderType, std::string& aShaderSource);
			TRITON_API unsigned int CreateProgram(std::initializer_list<unsigned int> aShaders);
		}
	}
}