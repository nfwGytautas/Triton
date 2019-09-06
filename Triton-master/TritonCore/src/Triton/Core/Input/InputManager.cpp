#include "TRpch.h"
#include "InputManager.h"

namespace Triton
{
	namespace Core
	{
		InputManager::InputManager()
		{
			m_keyboardState = new Keyboard();
			m_mouseState = new Mouse();
		}

		InputManager::~InputManager()
		{
			delete m_keyboardState;
			delete m_mouseState;
		}

		void InputManager::keyInput(InputType type, int key)
		{
			if (m_lastLey == key)
			{
				m_keyRepeat++;
			}
			else
			{
				m_keyRepeat = 0;
			}

			if (type == InputType::PRESSED)
			{
				m_keyboardState->Keys[key] = true;
				this->Post(new KeyPressedEvent(key, m_keyRepeat));
			}
			else
			{
				m_keyboardState->Keys[key] = false;
				this->Post(new KeyReleasedEvent(key));
			}

			m_lastLey = key;
		}

		void InputManager::charInput(unsigned int chr)
		{
			this->Post(new KeyInputEvent(chr));
		}

		void InputManager::mouseKeyInput(InputType type, int key)
		{
			if (type == InputType::PRESSED)
			{
				m_mouseState->Keys[key] = true;
				this->Post(new MouseButtonPressedEvent(key));
			}
			else
			{
				m_mouseState->Keys[key] = false;
				this->Post(new MouseButtonReleasedEvent(key));
			}
		}

		void InputManager::mouseMoveInput(double xdelta, double ydelta)
		{
			m_mouseState->XPosDelta = xdelta;
			m_mouseState->YPosDelta = ydelta;

			this->Post(new MouseMovedEvent(xdelta, ydelta));
		}

		void InputManager::mouseScrollInput(double xdelta, double ydelta)
		{
			m_mouseState->XScrollDelta = xdelta;
			m_mouseState->YScrollDelta = ydelta;

			m_mouseState->ScrollX += xdelta;
			m_mouseState->ScrollY += ydelta;

			this->Post(new MouseScrolledEvent(xdelta, ydelta));
		}

		relay_ptr<Keyboard> InputManager::getKeyboard()
		{
			return m_keyboardState;
		}

		relay_ptr<Mouse> InputManager::getMouse()
		{
			return m_mouseState;
		}

		void InputManager::gotFocus()
		{
			this->Post(new WindowFocusedEvent());
			m_focus = true;
		}

		void InputManager::lostFocus()
		{
			this->Post(new WindowLostFocusEvent());
			m_focus = false;

			// Disable all mouse keys
			// Only affects polling
			for (int i = 0; i < TR_MOUSE_KEY_COUNT; i++)
			{
				m_mouseState->Keys[i] = false;
			}

			// Disable all keyboard keys
			// Only affects polling
			for (int i = 0; i < TR_KEYBOARD_KEY_COUNT; i++)
			{
				m_keyboardState->Keys[i] = false;
			}
		}

		void InputManager::onMessage(size_t message, void* payload)
		{
			switch (message)
			{
			case (size_t)TritonMessageType::Update:
			{
				Dispatch();
				return;
			}
			}
		}


	}
}