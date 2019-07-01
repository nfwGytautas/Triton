#pragma once

#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXWindow : public Window
{
public:
	// Inherited via Window
	virtual void create(unsigned int width, unsigned height) override;
	virtual std::tuple<int, int> getWindowSize() override;
	virtual bool windowClosed() override;
	virtual void update() override;
	virtual void setVsync(bool value) override;
	virtual void showCursor(bool value) override;
	virtual void clear(float r, float g, float b, float a) override;
	virtual void destroy() override;

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
private:
	Core::InputManager* m_iManager;
	bool m_fullscreen = false;

	bool m_hasWindow;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	IDXGISwapChain* m_swapChain;

	// Current message from windows
	MSG m_msg;

	bool m_vsync;

	friend DXContext;
};

PLATFORM_NAMESPACE_END