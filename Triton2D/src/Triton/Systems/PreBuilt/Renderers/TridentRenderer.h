#pragma once

#include "..\..\..\TRMacros.h"
#include "..\..\Renderer2D.h"

namespace Triton {

	namespace Systems {

		class TRITON_API TridentRenderer : public Renderer2D
		{
		public:
			TridentRenderer();
			~TridentRenderer();

			//2D Mandatory methods
			virtual void Render(/*Arguments 1:RenderBatch*/) {}
			virtual void RenderUI(/*Arguments 1:UILayer*/) {}
			//2D Optional methods
			const char* RendererInformation() override;

		};

	}
}