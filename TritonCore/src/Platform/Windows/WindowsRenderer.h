#pragma once

#include "Triton\Core\Renderer\Renderer.h"
#include "Triton\Core\Renderer\RenderRoutine.h"

#include "WindowsModel.h"
#include "WindowsTexture.h"

namespace Triton
{
	namespace Core
	{
		class WindowsRenderer : public Renderer
		{
		public:
			WindowsRenderer();
			virtual ~WindowsRenderer() { }

			virtual void Render(unsigned int aIndiceCount) override;

		protected:
			virtual void Prepare() override;
		};
	}
}