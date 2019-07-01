#pragma once

#include "Triton/Events/EventManager.h"
#include "Triton/Events/Events.h"
#include "Triton/Events/KeyEvent.h"
#include "Triton/Events/MouseEvent.h"
#include "Triton/Events/ApplicationEvent.h"
#include "Triton/Events/EventInterface.h"

#include "Keyboard.h"
#include "Mouse.h"

namespace Triton
{
	namespace Core
	{
		enum class InputType : size_t
		{
			PRESSED = 0,
			RELEASED = 1
		};

		class InputManager
		{
		public:
			InputManager();
			virtual ~InputManager();

			void keyInput(InputType type, int key);
			void charInput(unsigned int chr);
			void mouseKeyInput(InputType type, int key);
			void mouseMoveInput(double xdelta, double ydelta);
			void mouseScrollInput(double xdelta, double ydelta);

			// Set the EventManager that receives direct changes
			void setEventManager(reference<Core::EventManager> manager);

			// Returns a relay_ptr to a input managers keyboard state
			relay_ptr<Keyboard> getKeyboard();

			// Returns a relay_ptr to a input managers mouse state
			relay_ptr<Mouse> getMouse();
		private:
			reference<Core::EventManager> m_eManager;

			Keyboard* m_keyboardState;
			Mouse* m_mouseState;

			unsigned int m_keyRepeat = 0;
			int m_lastLey = 0;
		};
	}
}