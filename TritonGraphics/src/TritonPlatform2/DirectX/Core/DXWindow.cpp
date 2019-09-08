#include "DXWindow.h"

#include <windowsx.h>
#include <shellapi.h>

#include "Triton\Logger\Log.h"
#include "Triton/Core/Input/InputManager.h"

static Triton::Graphics::DXWindow* WindowHandle;
static int WindowIndex = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (WindowHandle)
	{
		return WindowHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	else
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

namespace Triton
{
	namespace Graphics
	{
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
					m_focused = false;
				}
				else
				{
					m_focused = true;
				}
				return 0;
			}

			case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
			case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
			{
				SetCapture(m_hwnd);

				int button = 0;
				if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
				if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
				if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
				if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }

				m_mouseState->setKey((IO::MouseKey)button, true);
				return 0;
			}

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				ReleaseCapture();

				int button = 0;
				if (msg == WM_LBUTTONUP) { button = 0; }
				if (msg == WM_RBUTTONUP) { button = 1; }
				if (msg == WM_MBUTTONUP) { button = 2; }
				if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }

				m_mouseState->setKey((IO::MouseKey)button, false);
				return 0;
			}

			case WM_MOUSEWHEEL:
				m_mouseState->addXScrollDelta((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
				return 0;
			case WM_MOUSEHWHEEL:
				m_mouseState->addYScrollDelta((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
				return 0;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				if (wParam < 256)
				{
					m_keyboardState->setKey((IO::Key)wParam, true);
				}
				return 0;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				if (wParam < 256)
				{
					m_keyboardState->setKey((IO::Key)wParam, false);
				}
				return 0;

			case WM_CHAR:
				m_keyboardState->charInput((char)wParam);
				return 0;

			case WM_MOUSEMOVE:
				m_mouseState->setPosX(GET_X_LPARAM(lParam));
				m_mouseState->setPosY(GET_Y_LPARAM(lParam));
				return 0;

			case WM_DROPFILES:
			{
				HDROP hDrop = (HDROP)wParam;
				UINT nCnt = DragQueryFile(hDrop, (UINT)-1, NULL, 0);

				std::vector<std::string> dropedFiles;
				dropedFiles.reserve(nCnt);

				for (int nIndex = 0; nIndex < nCnt; ++nIndex) {
					UINT nSize;
					if (0 == (nSize = DragQueryFile(hDrop, nIndex, NULL, 0)))
					{
						continue;
					}

					TCHAR *pszFileName = new TCHAR[++nSize];
					if (DragQueryFile(hDrop, nIndex, pszFileName, nSize)) {
						std::wstring path = std::wstring(pszFileName);
						dropedFiles.push_back(std::string(path.begin(), path.end()));
					}
					delete[] pszFileName;
				}

				if (m_dragDropCallback)
				{
					m_dragDropCallback(dropedFiles);
				}

				DragFinish(hDrop);

				return 0;
			}

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
					m_mouseState->addXPosDelta(xPos);
					m_mouseState->addYPosDelta(yPos);
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

		DXWindow::DXWindow(IO::Keyboard* kBoard, IO::Mouse* mouse)
			: Window(kBoard, mouse)
		{
			m_initialized = false;
		}

		bool DXWindow::initNew(float startingWidth, float startingHeight)
		{
			if (m_initialized)
			{
				return false;
			}

			m_className = std::wstring(L"WindowClass" + std::to_wstring(WindowIndex)).c_str();
			m_hinstance = GetModuleHandle(NULL);
			WindowHandle = this;

			WNDCLASSEX wc = { 0 };
			DEVMODE dmScreenSettings;
			int posX, posY;

			// Determine the resolution of the clients desktop screen.
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			
			// Initialize the message structure.
			ZeroMemory(&m_msg, sizeof(MSG));

			TR_SYSTEM_INFO("Creating a WINDOWS display: W:{0} H:{1}", startingWidth, startingHeight);

			// Setup the windows class with default settings.
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = m_hinstance;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hIconSm = wc.hIcon;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = m_className;
			wc.cbSize = sizeof(WNDCLASSEX);

			// Register the window class.
			RegisterClassEx(&wc);

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
				screenWidth = startingWidth;
				screenHeight = startingHeight;

				// Place the window in the middle of the screen.
				posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
				posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
			}

			// Create the window with the screen settings and get the handle to it.
			m_hwnd = CreateWindowEx(NULL, m_className, L"Engine",
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance,
				NULL);
			//this);

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
				return false;
			}

			POINT p;
			if (GetCursorPos(&p))
			{
				m_mouseState->setPosX(p.x);
				m_mouseState->setPosY(p.y);
			}

			SetCapture(m_hwnd);

			m_initialized = true;
			m_selfCreated = true;
			return true;
		}

		bool DXWindow::initFromCreated(std::vector<void*> params)
		{
			// TODO: Implement
			m_initialized = false;
			m_selfCreated = false;
			return false;
		}

		void DXWindow::update()
		{
			// Handle the windows messages.
			while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
			{
				if (m_msg.message == WM_QUIT)
				{
					m_closed = true;
					return;
				}

				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}

		void DXWindow::destroy()
		{
			if (m_selfCreated)
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
				UnregisterClass(m_className, m_hinstance);
				m_hinstance = NULL;

				// Release the pointer to this class.
				if (WindowHandle == this)
				{
					WindowHandle = NULL;
				}

				return;
			}
		}

		std::tuple<int, int> DXWindow::size()
		{
			RECT rcClient;
			GetClientRect(m_hwnd, &rcClient);
			int width = rcClient.right - rcClient.left;
			int height = rcClient.bottom - rcClient.top;

			return { width, height };
		}

		void DXWindow::showCursor(bool value)
		{
			ShowCursor(value);
		}

		void DXWindow::makeCurrent()
		{
			WindowHandle = this;
		}

		HWND DXWindow::nativeHandle()
		{
			return m_hwnd;
		}

		void DXWindow::setCursorPos(double x, double y)
		{
			POINT pt;
			pt.x = x;
			pt.y = y;
			ClientToScreen(m_hwnd, &pt);
			SetCursorPos(pt.x, pt.y);
		}
	}
}