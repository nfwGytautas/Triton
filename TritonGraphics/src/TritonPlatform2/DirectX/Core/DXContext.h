#pragma once

#include "Triton/AppSettings.h"

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Core/Context.h"

PLATFORM_NAMESPACE_BEGIN

class DXContext : public Context
{
public:
	/// Create the DX context
	DXContext();

	// Inherited via Context
	virtual Window* newWindow() override;
	virtual Renderer* newRenderer(Window* window) override;

	virtual bool init() override;
	virtual void shutdown() override;

	virtual reference<Shader> newShader(const ShaderCreateParams & createParams) override;
	virtual reference<VAO> newVAO(const IO::MeshData::Mesh & createParams) override;
	virtual reference<Texture> newTexture(const IO::ImageData & createParams) override;
	virtual reference<CubeTexture> newCubeTexture(const IO::ImageArrayData & createParams) override;
	virtual reference<Framebuffer> newFramebuffer(unsigned int width, unsigned int height) override;
	virtual reference<Bitmap> newBitmap(const BitmapCreateParams & createParams) override;
private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	std::vector<DXGI_MODE_DESC> m_displayModeList;

	std::string m_appNameSTD;
	LPCWSTR m_appName;

	int m_videoCardMemory;
	char m_videoCardDescription[128];
};

PLATFORM_NAMESPACE_END