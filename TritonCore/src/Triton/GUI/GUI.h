#pragma once

#ifndef TR_DISABLE_GUI

#include "Triton\TRMacros.h"
#include "imgui.h"

namespace Triton
{
	namespace UI
	{

		class TRITON_API GUI
		{
		public:
			GUI(unsigned int aDisplayWidth, unsigned int aDisplayHeight);
			~GUI();

			//Define how a GUI looks
			virtual void Visualize() = 0;
			virtual void Update(float aDelta) = 0;

			void Refresh();
			void Draw();

			ImGuiIO& IO();
		};

	}
}

#endif