#pragma once
#include "Triton\TRMacros.h"
#include "Events.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API EventReceiver
		{
		public:
			virtual ~EventReceiver() {}

			virtual void OnEvent(Event* aEvent) = 0;

			virtual void OnKeyPressed(int aKeycode, int aRepeatCount, int aScancode, int aMods);
			virtual void OnKeyReleased(int aKeycode, int aScancode, int aMods);
			virtual void OnMouseMoved(double aX, double aY);
			virtual void OnMouseButtonPressed(int aKeycode);
			virtual void OnMouseButtonReleased(int aKeycode);
			virtual void OnMouseScrolled(double aXOffset, double aYOffset);
			virtual void OnKeyInput(unsigned int aCharInput);
			virtual void OnWindowResized(int aWidth, int aHeight);
		};
	}
}