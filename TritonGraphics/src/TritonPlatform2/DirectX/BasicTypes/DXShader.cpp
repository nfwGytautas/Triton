#include "DXShader.h"

PLATFORM_NAMESPACE_BEGIN

DXShader::DXShader()
{
}

DXShader::~DXShader()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void DXShader::enable()
{
	// Set the vertex input layout.
	m_deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	m_deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	enabled = true;
}

void DXShader::disable()
{

	enabled = false;
}

void DXShader::mapBuffer(ID3D11Buffer* buffer, D3D11_MAPPED_SUBRESOURCE& mappedResource)
{
	HRESULT result;

	// Lock the light constant buffer so it can be written to.
	result = m_deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_SYSTEM_ERROR("Locking of constant buffer failed!");
		return;
	}
}

void DXShader::unmapBuffer(ID3D11Buffer* buffer)
{
	// Unlock the constant buffer.
	m_deviceContext->Unmap(buffer, 0);
}

void DXShader::update_matrices()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;

	mapBuffer(m_matrixBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<MatrixBuffer*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	*dataPtr = prtc_Matrices;

	unmapBuffer(m_matrixBuffer);

	m_deviceContext->VSSetConstantBuffers(c_MatrixBufferSlot, 1, &m_matrixBuffer);
}

PLATFORM_NAMESPACE_END