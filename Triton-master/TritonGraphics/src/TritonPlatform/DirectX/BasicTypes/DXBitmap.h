#pragma once

#include "TritonPlatform/DirectX/Macros.h"

PLATFORM_NAMESPACE_BEGIN

class DXBitmap : public Bitmap
{
public:
	virtual ~DXBitmap();

	virtual void enable() override;
	virtual void disable() override;

	virtual void setPosition(unsigned int newX, unsigned int newY) override;
	virtual unsigned int getIndiceCount() override;
protected:
	// Inherited via Bitmap
	virtual void create(FactoryCreateParams * createParams) override;
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	unsigned int m_vertexCount;
	unsigned int m_indiceCount;

	friend DXRenderer;
	friend DXFactory;
};

PLATFORM_NAMESPACE_END