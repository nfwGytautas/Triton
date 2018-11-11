#pragma once

#include "..\TRMacros.h"

namespace Triton {

	namespace Systems {

		//A renderer class that allows for creation of custom 2D renderers
		class TRITON_API Renderer
		{
		public:
			Renderer() {}
			virtual ~Renderer() {}

			//Mandatory methods
			virtual void Render(/*Arguments 1:RenderBatch*/) = 0;
			virtual void RenderUI(/*Arguments 1:UILayer*/) = 0;
			//Optional methods
			virtual const char* RendererInformation() { return "Information not given"; };
		};

	}
}