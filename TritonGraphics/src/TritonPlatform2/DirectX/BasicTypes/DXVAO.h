#pragma once

#include "TritonPlatform2/DirectX/Macros.h"
#include "TritonPlatform2/CrossTypes/VAO.h"

PLATFORM_NAMESPACE_BEGIN

class DXRenderer;
class DXContext;

class DXVAO : public VAO
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};
public:
	virtual ~DXVAO();

	virtual void enable() override;
	virtual void disable() override;

	virtual unsigned int getIndiceCount() override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	unsigned int m_indiceCount;

	friend DXRenderer;
	friend DXContext;
};

PLATFORM_NAMESPACE_END