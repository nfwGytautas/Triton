#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

PLATFORM_NAMESPACE_BEGIN

class DXContext;

class DXShader : public Shader
{
	const int c_MatrixBufferSlot = 0;
public:
	DXShader();
	virtual ~DXShader();

	// Inherited via Shader
	virtual void enable() override;
	virtual void disable() override;
	virtual void update_matrices() override;
private:
	void mapBuffer(ID3D11Buffer* buffer, D3D11_MAPPED_SUBRESOURCE& mappedResource);
	void unmapBuffer(ID3D11Buffer* buffer);
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_matrixBuffer;

	friend DXContext;
};

PLATFORM_NAMESPACE_END