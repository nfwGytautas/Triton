#include "Display.h"

#include "Triton\Logger\Log.h"
#include "Triton\Core\GraphicsAPI.h"

namespace Triton {
	namespace Core {

		Display::Display()
		{
		}


		Display::~Display()
		{
		}

		bool Display::Closed()
		{
			return glfwWindowShouldClose(GraphicsAPI::ContextManager::GetContext());
		}

		void Display::Update()
		{
			//Check errors
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				TR_CORE_WARN("OpenGL error: {0}", error);
			}

			glfwSwapBuffers(GraphicsAPI::ContextManager::GetContext());
		}

	}
}