#include "DXBitmap.h"

#include "DXTexture.h"

PLATFORM_NAMESPACE_BEGIN

DXBitmap::~DXBitmap()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Triton::PType::DXBitmap::enable()
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(BitmapVertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Texture->enable();

	return;
}

void Triton::PType::DXBitmap::disable()
{
}

void DXBitmap::create(FactoryCreateParams * createParams)
{
	prtc_previousPosX = -1;
	prtc_previousPosY = -1;
}

unsigned int DXBitmap::getIndiceCount()
{
	return m_indiceCount;
}

void Triton::PType::DXBitmap::setPosition(unsigned int newX, unsigned int newY)
{
	float left, right, top, bottom;
	BitmapVertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	BitmapVertexType* verticesPtr;
	HRESULT result;

	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if ((newX == prtc_previousPosX) && (newY == prtc_previousPosY))
	{
		return;
	}

	// If it has changed then update the position it is being rendered to.
	prtc_previousPosX = newX;
	prtc_previousPosY = newY;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((prtc_screenWidth / 2) * -1) + (float)newX;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)prtc_bitmapWidth;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(prtc_screenHeight / 2) - (float)newY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)prtc_bitmapHeight;

	// Create the vertex array.
	vertices = new BitmapVertexType[m_vertexCount];

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = Vector3(left, top, 0.0f);  // Top left.
	vertices[0].uv = Vector2(0.0f, 0.0f);

	vertices[1].position = Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].uv = Vector2(1.0f, 1.0f);

	vertices[2].position = Vector3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].uv = Vector2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = Vector3(left, top, 0.0f);  // Top left.
	vertices[3].uv = Vector2(0.0f, 0.0f);

	vertices[4].position = Vector3(right, top, 0.0f);  // Top right.
	vertices[4].uv = Vector2(1.0f, 0.0f);

	vertices[5].position = Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].uv = Vector2(1.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	result = m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_CORE_ERROR("Failed to map bitmap vertexbuffer");
		return;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (BitmapVertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(BitmapVertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	m_deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

}

PLATFORM_NAMESPACE_END