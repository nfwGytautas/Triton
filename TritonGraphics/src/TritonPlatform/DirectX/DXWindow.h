#pragma once

#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXWindow : public Window
{
public:
	// Inherited via Window
	virtual void create() override;
	virtual std::tuple<int, int> getWindowSize() override;
	virtual bool windowClosed() override;
	virtual void update() override;
	virtual void showCursor(bool value) override;
	virtual void destroy() override;

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
private:
	reference<Triton::Core::InputManager> m_iManager;
	bool m_fullscreen = false;

	bool m_hasWindow;
	bool m_selfCreated = false;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;	

	// Current message from windows
	MSG m_msg;

	unsigned int m_width;
	unsigned int m_height;

	friend DXContext;
};

PLATFORM_NAMESPACE_END