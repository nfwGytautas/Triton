#include "Window.h"

namespace Triton
{
	namespace Graphics
	{
		Window::Window(IO::Keyboard* kBoard, IO::Mouse* mouse)
			: m_keyboardState(kBoard), m_mouseState(mouse)
		{
		}

		bool Window::isClosed()
		{
			return m_closed;
		}

		bool Window::isFullscreen()
		{
			return m_fullscreen;
		}

		bool Window::isFocused() const
		{
			return m_focused;
		}

		void Window::setDropCallback(std::function<void(std::vector<std::string>)> callback)
		{
			m_dragDropCallback = callback;
		}

		IO::Keyboard& Window::keyboard() const
		{
			return *m_keyboardState;
		}

		IO::Mouse& Window::mouse() const
		{
			return *m_mouseState;
		}
	}
}