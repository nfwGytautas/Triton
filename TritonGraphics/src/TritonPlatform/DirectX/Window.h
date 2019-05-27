#pragma once

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

static Triton::PType::DXWindow* WindowHandle;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return WindowHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN

	// Creates the window
	inline void DXWindow::create(unsigned int width, unsigned height)
	{
		TR_CORE_INFO("Creating a WINDOWS display: W:{0} H:{1}", width, height);

		// Initialize the message structure.
		ZeroMemory(&m_msg, sizeof(MSG));

		m_applicationName = L"Engine";

		m_hinstance = GetModuleHandle(NULL);

		WindowHandle = this;

		WNDCLASSEX wc = { 0 };
		DEVMODE dmScreenSettings;
		int posX, posY;

		// Setup the windows class with default settings.
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_applicationName;
		wc.cbSize = sizeof(WNDCLASSEX);

		// Register the window class.
		RegisterClassEx(&wc);

		// Determine the resolution of the clients desktop screen.
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (m_fullscreen)
		{
			// If full screen set the screen to maximum size of the users desktop and 32bit.
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the display settings to full screen.
			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			// Set the position of the window to the top left corner.
			posX = posY = 0;
		}
		else
		{
			screenWidth = width;
			screenHeight = height;

			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		}

		// Create the window with the screen settings and get the handle to it.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);

		// Hide the mouse cursor.
		ShowCursor(false);

		return;
	}

	// Get window size
	inline std::tuple<int, int> DXWindow::getWindowSize()
	{
		RECT rcClient;
		GetClientRect(m_hwnd, &rcClient);
		int width = rcClient.right - rcClient.left;
		int height = rcClient.bottom - rcClient.top;

		return { width, height };
	}

	// Check if the window is closed
	inline bool DXWindow::windowClosed()
	{
		return (m_msg.message == WM_QUIT);
	}

	// Update the context
	inline void DXWindow::update()
	{
		// Handle the windows messages.
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}

		// Present the back buffer to the screen since rendering is complete.
		if (m_vsync)
		{
			// Lock to screen refresh rate.
			m_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			m_swapChain->Present(0, 0);
		}
	}

	// Set window vsync property
	inline void DXWindow::setVsync(bool value)
	{
		m_vsync = value;
		//glfwSwapInterval((int)value);
	}

	// Set cursor visability
	inline void DXWindow::showCursor(bool value)
	{
		ShowCursor(value);
	}

	// Clear window contents
	inline void Triton::PType::DXWindow::clear(float r, float g, float b, float a)
	{
		
	}

	// Destroys the window
	inline void DXWindow::destroy()
	{
		// Show the mouse cursor.
		ShowCursor(true);

		// Fix the display settings if leaving full screen mode.
		if (m_fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		// Remove the window.
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		// Remove the application instance.
		UnregisterClass(m_applicationName, m_hinstance);
		m_hinstance = NULL;

		// Release the pointer to this class.
		WindowHandle = NULL;

		return;
	}

	inline LRESULT CALLBACK DXWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		static unsigned int prevKey = 0;
		static int repeat = 0;

		switch (umsg)
		{
			// Check if a key has been pressed on the keyboard.
			case WM_KEYDOWN:
			{
				if (prevKey == (unsigned int)wparam)
				{
					repeat++;
				}
				else
				{
					prevKey = (unsigned int)wparam;
					repeat = 0;
				}
				m_receiver->OnKeyPressed((unsigned int)wparam, repeat, -1, 0);
				return 0;
			}

			// Check if a key has been released on the keyboard.
			case WM_KEYUP:
			{			
				m_receiver->OnKeyReleased((unsigned int)wparam, -1, 0);
				return 0;
			}

			// Any other messages send to the default message handler as our application won't make use of them.
			default:
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
		}
	}

NAMESPACE_END