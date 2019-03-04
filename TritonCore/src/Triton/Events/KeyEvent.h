#pragma once

#include "Events.h"

namespace Triton {

	class TRITON_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode), m_Mods(0) {}

		KeyEvent(int keycode, int mods)
			: m_KeyCode(keycode), m_Mods(mods) {}

		int m_KeyCode;
		int m_Mods;
	};

	class TRITON_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode, 0), m_RepeatCount(repeatCount) {}

		KeyPressedEvent(int keycode, int repeatCount, int scancode, int mods)
			: KeyEvent(keycode, mods), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class TRITON_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode, 0) {}

		KeyReleasedEvent(int keycode, int scancode, int mods)
			: KeyEvent(keycode, mods) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class TRITON_API KeyInputEvent : public KeyEvent
	{
	public:
		KeyInputEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyInputEvent)
	};
}