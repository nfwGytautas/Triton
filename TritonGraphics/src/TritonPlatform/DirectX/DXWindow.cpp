#include "DXWindow.h"

#include <windowsx.h>

#include "Triton\Logger\Log.h"
#include "Triton/Core/Input/InputManager.h"

static Triton::PType::DXWindow* WindowHandle;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return WindowHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}

PLATFORM_NAMESPACE_BEGIN

// Creates the window
void DXWindow::create(unsigned int width, unsigned height)
{
	if (m_hasWindow)
	{
		return;
	}

	TR_SYSTEM_INFO("Creating a WINDOWS display: W:{0} H:{1}", width, height);

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
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, this);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;   // default
	//Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;   // default
	//Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
		TR_SYSTEM_ERROR("RID registration failure");
		return;
	}

	POINT p;
	if (GetCursorPos(&p))
	{
		m_iManager->getMouse()->X = p.x;
		m_iManager->getMouse()->Y = p.y;
	}

	return;
}

// Get window size
std::tuple<int, int> DXWindow::getWindowSize()
{
	RECT rcClient;
	GetClientRect(m_hwnd, &rcClient);
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	return { width, height };
}

// Check if the window is closed
bool DXWindow::windowClosed()
{
	return (m_msg.message == WM_QUIT);
}

// Update the context
void DXWindow::update()
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
void DXWindow::setVsync(bool value)
{
	m_vsync = value;
	//glfwSwapInterval((int)value);
}

// Set cursor visability
void DXWindow::showCursor(bool value)
{
	ShowCursor(value);
}

// Clear window contents
void Triton::PType::DXWindow::clear(float r, float g, float b, float a)
{

}

// Destroys the window
 void DXWindow::destroy()
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

LRESULT CALLBACK DXWindow::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static unsigned int prevKey = 0;
	static int repeat = 0;

	switch (msg)
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


		case WM_ACTIVATE:
		{
			if (wParam == WA_INACTIVE)
			{
				m_iManager->lostFocus();
			}
			else
			{
				m_iManager->gotFocus();
			}
			return 0;
		}

		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
		{
			int button = 0;
			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
			if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
			if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
			if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		
			m_iManager->mouseKeyInput(Triton::Core::InputType::PRESSED, button);
			return 0;
		}
		
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			int button = 0;
			if (msg == WM_LBUTTONUP) { button = 0; }
			if (msg == WM_RBUTTONUP) { button = 1; }
			if (msg == WM_MBUTTONUP) { button = 2; }
			if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		
			m_iManager->mouseKeyInput(Triton::Core::InputType::RELEASED, button);
			return 0;
		}
		
		case WM_MOUSEWHEEL:
			m_iManager->mouseScrollInput((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, 0);
			return 0;
		case WM_MOUSEHWHEEL:	
			m_iManager->mouseScrollInput(0, (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
			return 0;
		
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (wParam < 256)
			{
				m_iManager->keyInput(Triton::Core::InputType::PRESSED, wParam);
			}
			return 0;
		
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (wParam < 256)
			{
				m_iManager->keyInput(Triton::Core::InputType::RELEASED, wParam);
			}
			return 0;
		
		case WM_CHAR:
			m_iManager->charInput((unsigned int)wParam);
			return 0;

		case WM_MOUSEMOVE:
			m_iManager->getMouse()->X = GET_X_LPARAM(lParam);
			m_iManager->getMouse()->Y = GET_Y_LPARAM(lParam);
			return 0;
		
		case WM_INPUT:
		{
			UINT dwSize;

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
				sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == NULL)
			{
				return 0;
			}

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
				sizeof(RAWINPUTHEADER)) != dwSize)
			{
				TR_SYSTEM_ERROR("GetRawInputData does not return correct size!");
			}

			RAWINPUT* raw = (RAWINPUT*)lpb;

			LPWSTR szTempOutput = LPWSTR(1000);

			// Only handle mouse move events more specifically the mouse delta
			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				double xPos = raw->data.mouse.lLastX;
				double yPos = raw->data.mouse.lLastY;
				m_iManager->mouseMoveInput(xPos, yPos);
			}

			delete[] lpb;
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}

PLATFORM_NAMESPACE_END