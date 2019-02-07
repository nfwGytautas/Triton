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

			virtual void Render(std::vector<Data::RenderBatch>& aRenderBatch) override;
		private:
			void Prepare();
			void SetDefaultValues();
		private:
			WindowsShader* m_Shader;
		};
	}
}