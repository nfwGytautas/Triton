#pragma once

#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXFactory : public Factory
{
public:
	reference<FactoryObject> createShader(FactoryCreateParams* createParams) override;
	reference<FactoryObject> createVAO(FactoryCreateParams* createParams) override;
	reference<FactoryObject> createTexture(FactoryCreateParams* createParams) override;
	reference<FactoryObject> createFramebuffer(FactoryCreateParams* createParams) override;
	reference<FactoryObject> createBitmap(FactoryCreateParams* createParams) override;
private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
private:
	HWND m_hwnd;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11DepthStencilView* m_depthStencilView;

	friend DXContext;
};

PLATFORM_NAMESPACE_END