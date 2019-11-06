#include "DXVAO.h"
#include "TritonTypes/IO.h"
#include "Triton2/Utility/Log.h"

namespace Triton
{
	namespace Graphics
	{

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

		void DXVAO::enable()
		{
			unsigned int stride;
			unsigned int offset;


			// Set vertex buffer stride and offset.
			stride = sizeof(IO::Vertex);
			offset = 0;

			// Set the vertex buffer to active in the input assembler so it can be rendered.
			m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			return;
		}

		void DXVAO::disable()
		{
		}

		unsigned int DXVAO::getIndiceCount()
		{
			return m_indiceCount;
		}

		void DXVAO::updateVertices(std::vector<IO::Vertex>& vertices)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			IO::Vertex* verticesPtr;

			// Lock the vertex buffer so it can be written to.
			result = m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to map vertex array buffer");
				return ;
			}

			// Get a pointer to the data in the vertex buffer.
			verticesPtr = (IO::Vertex*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertices.data(), (sizeof(IO::Vertex) * vertices.size()));

			// Unlock the vertex buffer.
			m_deviceContext->Unmap(m_vertexBuffer, 0);
		}

		void DXVAO::updateIndices(std::vector<unsigned int>& indices)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			unsigned int* indicesPtr;

			// Lock the indices buffer so it can be written to.
			result = m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to map vertex array buffer");
				return;
			}

			// Get a pointer to the data in the vertex buffer.
			indicesPtr = (unsigned int*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(indicesPtr, (void*)indices.data(), (sizeof(IO::Vertex) * indices.size()));

			// Unlock the vertex buffer.
			m_deviceContext->Unmap(m_vertexBuffer, 0);
		}

	}
}