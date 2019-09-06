#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Core/Window.h"

namespace Triton
{
	namespace Graphics
	{
		class DXWindow : public Window
		{
		public:
			LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
		public:
			DXWindow(IO::Keyboard* kBoard, IO::Mouse* mouse);

			// Inherited via Window
			virtual bool initNew(float startingWidth, float startingHeight) override;
			virtual bool initFromCreated(std::vector<void*> params) override;

			virtual void update() override;
			virtual void destroy() override;

			virtual void setCursorPos(double x, double y) override;

			virtual void showCursor(bool value) override;
			virtual std::tuple<int, int> size() override;

			virtual void makeCurrent() override;

			/**
			 * Get the native handle to the window
			 *
			 * @return The HWND of the window
			 */
			HWND nativeHandle();
		private:
			HWND m_hwnd;
			HINSTANCE m_hinstance;
			MSG m_msg;

			LPCWSTR m_className;
		};
	}
}