#include "DXVAO.h"

PLATFORM_NAMESPACE_BEGIN

DXVAO::~DXVAO()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Triton::PType::DXVAO::enable()
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void Triton::PType::DXVAO::disable()
{
}

void Triton::PType::DXVAO::create(FactoryCreateParams* createParams)
{
	auto glParams = OBJECT_AS(VAOCreateParams, createParams);

	//m_indiceCount = glParams->indices.size();
}

unsigned int DXVAO::getIndiceCount()
{
	return m_indiceCount;
}

PLATFORM_NAMESPACE_END