#pragma once

#include "..\..\..\TRMacros.h"
#include "..\..\Renderer.h"

namespace Triton {

	namespace Systems {

		class TRITON_API TridentRenderer : public Renderer
		{
		public:
			TridentRenderer();
			~TridentRenderer();

			//Mandatory methods
			virtual void Render(/*Arguments 1:RenderBatch*/) {}
			virtual void RenderUI(/*Arguments 1:UILayer*/) {}
			//Optional methods
			const char* RendererInformation() override;

		};

	}
}