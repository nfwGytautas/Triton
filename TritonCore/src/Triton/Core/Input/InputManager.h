#pragma once

#include "Triton/Core/TritonClass.h"

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

		class InputManager : public TritonClass, public EventManager
		{
		public:
			InputManager();
			virtual ~InputManager();

			void keyInput(InputType type, int key);
			void charInput(unsigned int chr);
			void mouseKeyInput(InputType type, int key);
			void mouseMoveInput(double xdelta, double ydelta);
			void mouseScrollInput(double xdelta, double ydelta);

			// Returns a relay_ptr to a input managers keyboard state
			relay_ptr<Keyboard> getKeyboard();

			// Returns a relay_ptr to a input managers mouse state
			relay_ptr<Mouse> getMouse();

			// Notify that the focus has been regained
			// Sends gained focus event
			void gotFocus();

			// Notify that the focus has been lost
			// Sends lost focus event
			void lostFocus();

			// Returns the state of the engine window
			// returns [true] if focused
			inline bool focused() const
			{
				return m_focus;
			}

			// Inherited via TritonClass
			virtual void onRegistered() override;
			virtual void onUnRegistered() override;
		private:
			Keyboard* m_keyboardState;
			Mouse* m_mouseState;

			unsigned int m_keyRepeat = 0;
			int m_lastLey = 0;

			bool m_focus = true;
		};
	}
}