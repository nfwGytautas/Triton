#include "Mouse.h"

namespace Triton
{
	namespace IO
	{
		std::tuple<double, double> Mouse::scroll() const
		{
			return std::tuple<double, double>(m_scrollX, m_scrollY);
		}

		std::tuple<double, double> Mouse::deltaScroll() const
		{
			return std::tuple<double, double>(m_xScrollDelta, m_yScrollDelta);
		}

		std::tuple<double, double> Mouse::position() const
		{
			return std::tuple<double, double>(m_x, m_y);
		}

		std::tuple<double, double> Mouse::deltaPosition() const
		{
			return std::tuple<double, double>(m_xPosDelta, m_yPosDelta);
		}

		bool Mouse::isKeyDown(MouseKey key) const
		{
			return m_keys[(size_t)key];
		}

		void Mouse::addXScrollDelta(double delta)
		{
			m_xScrollDelta = delta;
			m_scrollX += delta;
		}

		void Mouse::addYScrollDelta(double delta)
		{
			m_yScrollDelta = delta;
			m_scrollY += delta;
		}

		void Mouse::setPosX(double newVal)
		{
			m_x = newVal;
		}

		void Mouse::setPosY(double newVal)
		{
			m_y = newVal;
		}

		void Mouse::addPosDelta(double xDelta, double yDelta)
		{
			m_xPosDelta = xDelta;
			m_yPosDelta = yDelta;

			if (m_mouseMoveCallback)
			{
				m_mouseMoveCallback(xDelta, yDelta);
			}
		}

		void Mouse::setKey(MouseKey key, bool state)
		{
			m_keys[(size_t)key] = state;

			if (state)
			{
				if (m_mouseDownCallback)
				{
					m_mouseDownCallback(key);
				}
			}
			else
			{
				if (m_mouseUpCallback)
				{
					m_mouseUpCallback(key);
				}
			}
			
		}

		void Mouse::mouseDownCallback(std::function<void(MouseKey)> callback)
		{
			m_mouseDownCallback = callback;
		}

		void Mouse::mouseUpCallback(std::function<void(MouseKey)> callback)
		{
			m_mouseUpCallback = callback;
		}

		void Mouse::mouseMoveCallback(std::function<void(double, double)> callback)
		{
			m_mouseMoveCallback = callback;
		}
	}
}