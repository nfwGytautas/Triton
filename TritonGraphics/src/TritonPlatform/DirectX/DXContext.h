#pragma once

#include "Triton/AppSettings.h"
#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXContext : public Context
{
	DXWindow* dx_window();

	DXRenderer* dx_renderer();

	DXFactory* dx_factory();
public:
	Core::EventReceiver* receiver;
public:
	DXContext(const Triton::AppSettings& appSettings);
	virtual ~DXContext() { }

	// Inherited via Context
	virtual bool init() override;
	virtual bool init_additional() override;
	virtual void shutdown() override;
	virtual void setContextEventCallBacks(Core::EventReceiver* receiver) override;
	virtual void update() override;
	virtual void setViewPort(int x, int y, int width, int height) override;
	virtual void fillPacket(PlatformObject* packet) override;
	virtual void depthBufferState(bool state) override;

private:
	std::string m_appNameSTD;
	LPCWSTR m_appName;

	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
};

PLATFORM_NAMESPACE_END