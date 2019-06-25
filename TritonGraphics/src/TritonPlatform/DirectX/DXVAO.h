#pragma once

#include "Macros.h"

PLATFORM_NAMESPACE_BEGIN

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
protected:
	// Inherited via VAO
	virtual void create(FactoryCreateParams* createParams) override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	unsigned int m_indiceCount;

	friend DXRenderer;
	friend DXFactory;
};

PLATFORM_NAMESPACE_END