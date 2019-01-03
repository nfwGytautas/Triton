#pragma once
#include "Triton\Core\Renderer\Renderer.h"
#include "WindowsShader.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API WindowsRenderer : public Renderer
		{
		public:
			WindowsRenderer(WindowsShader* aShader);
			~WindowsRenderer();

			virtual void Render(ECS::Registry& aRegistry) override;
		private:
			WindowsShader* m_Shader;
		};
	}
}