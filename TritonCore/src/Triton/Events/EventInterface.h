#pragma once

#include "Events.h"

namespace Triton
{
	namespace Utility
	{
		// Helper class that provides virtual function declarations for a inherited class, aswell as registering itself with a manager
		// Note: This class should be avoided if performance is a concern since it adds a layer of virtual function calls and 
		// secondly you might not need all the possible events
		class EventInterface : public EventListener
		{
		public:
			EventInterface();
			virtual ~EventInterface();
		protected:
			virtual bool OnKeyPressed(int aKeycode, int aRepeatCount, int aScancode, int aMods) = 0;
			virtual bool OnKeyReleased(int aKeycode, int aScancode, int aMods) = 0;
			virtual bool OnKeyInput(unsigned int aCharInput) = 0;

			virtual bool OnMouseMoved(double aX, double aY) = 0;
			virtual bool OnMouseButtonPressed(int aKeycode) = 0;
			virtual bool OnMouseButtonReleased(int aKeycode) = 0;
			virtual bool OnMouseScrolled(double aXOffset, double aYOffset) = 0;

			virtual bool OnWindowResized(int aWidth, int aHeight) = 0;
		};
	}
}