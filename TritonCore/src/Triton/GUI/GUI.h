#pragma once

#include "Triton\TRMacros.h"
#include "imgui.h"

namespace Triton
{
	namespace UI
	{

		class TRITON_API GUI
		{
		public:
			GUI();
			~GUI();

			//Define how a GUI looks
			virtual void Visualize() = 0;
			virtual void Update(float aDelta) = 0;

			static void Refresh();
			static void Draw();

			ImGuiIO& IO();

			bool IsActive();
			void Disable();
			void Enable();

			bool IsOpen;
		private:
			bool mActive;
		};

	}
}