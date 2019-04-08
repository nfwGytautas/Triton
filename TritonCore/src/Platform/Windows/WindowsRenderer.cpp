#include "TRpch.h"
#include "WindowsRenderer.h"

#include <glad\glad.h>
#include "WindowsModel.h"
#include "WindowsTexture.h"
#include "Triton\Logger\Log.h"

#include "Triton\Core\Platform.h"

namespace Triton
{
	std::unique_ptr<Triton::Core::Renderer> Platform::Create()
	{
		return std::make_unique<Triton::Core::WindowsRenderer>();
	}

	namespace Core
	{
		WindowsRenderer::WindowsRenderer()
		{
		}

		void WindowsRenderer::Prepare()
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void WindowsRenderer::Render(unsigned int aIndiceCount)
		{
			glDrawElements(GL_TRIANGLES, aIndiceCount, GL_UNSIGNED_INT, (void*)0);
		}

	}
}