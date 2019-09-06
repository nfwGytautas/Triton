#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

PLATFORM_NAMESPACE_BEGIN

class DXContext;

class DXShader : public Shader
{
public:
	DXShader(ShaderLayout* layout);
	virtual ~DXShader();

	// Inherited via Shader
	virtual void enable() override;
	virtual void disable() override;

protected:
	DXShader()
	{ }

private:
	void updateBuffer(Shader::Buffer& buffer);

	ID3D11Buffer* getBuffer(const std::string& name);
	void mapBuffer(ID3D11Buffer* buffer, D3D11_MAPPED_SUBRESOURCE& mappedResource);
	void unmapBuffer(ID3D11Buffer* buffer);
	void setBuffer(ID3D11Buffer* buffer, const BufferShaderType& type, unsigned int count, unsigned int number);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11SamplerState* m_sampleState;

	std::unordered_map<std::string, ID3D11Buffer*> m_buffers;

	friend DXContext;

	// Inherited via Shader
	virtual void updateBuffers(BufferUpdateType type) override;
};

PLATFORM_NAMESPACE_END