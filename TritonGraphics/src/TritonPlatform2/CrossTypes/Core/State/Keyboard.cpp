#include "Keyboard.h"

namespace Triton
{
	namespace IO
	{
		bool Keyboard::isKeyDown(Key key) const
		{
			return m_keys[(size_t)key];
		}

		void Keyboard::setKey(Key key, bool state)
		{
			if ((int)key < TR_KEYBOARD_KEY_COUNT)
			{
				m_keys[(size_t)key] = state;
			}
		}

		void Keyboard::charInput(char input) const
		{
			if (m_charCallback)
			{
				m_charCallback(input);
			}
		}

		void Keyboard::charInputCallback(std::function<void(char)> callback)
		{
			m_charCallback = callback;
		}
	}
}