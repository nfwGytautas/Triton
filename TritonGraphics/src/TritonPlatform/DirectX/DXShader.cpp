#include "DXShader.h"

PLATFORM_NAMESPACE_BEGIN

DXShader::DXShader(ShaderLayout* layout)
	: Shader(layout)
{
}

DXShader::~DXShader()
{
	// Release all shader buffers
	for (auto &[name, buffer] : m_buffers) {
		buffer->Release();
		buffer = 0;
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

void DXShader::create(FactoryCreateParams* createParams)
{
	//TR_CORE_INFO("Loading shader complete. ID: {0}", m_shaderID);
}

ID3D11Buffer* DXShader::getBuffer(const std::string& name)
{
	ID3D11Buffer* d3dbuffer = m_buffers[name];

	if (d3dbuffer == nullptr)
	{
		TR_CORE_ERROR("Buffer of name {0} does not exist.", name);
		TR_CORE_ASSERT(d3dbuffer, "Buffer does not exist.")
			return NULL;
	}

	return d3dbuffer;
}

void DXShader::mapBuffer(ID3D11Buffer * buffer, D3D11_MAPPED_SUBRESOURCE& mappedResource)
{
	HRESULT result;

	// Lock the light constant buffer so it can be written to.
	result = m_deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}
}

void DXShader::unmapBuffer(ID3D11Buffer* buffer)
{
	// Unlock the constant buffer.
	m_deviceContext->Unmap(buffer, 0);
}

void DXShader::setBuffer(ID3D11Buffer* buffer, const BufferShaderType& type, unsigned int count, unsigned int number)
{
	if (type == BufferShaderType::PIXEL)
	{
		m_deviceContext->PSSetConstantBuffers(number, count, &buffer);
	}
	else if (type == BufferShaderType::VERTEX)
	{
		m_deviceContext->VSSetConstantBuffers(number, count, &buffer);
	}
}

void DXShader::updateBuffer(Shader::Buffer& buffer)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned char* dataPtr;

	ID3D11Buffer* d3dbuffer = getBuffer(buffer.layout.getName());

	mapBuffer(d3dbuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<unsigned char*>(mappedResource.pData);

	// Copy the lighting variables into the constant buffer.
	buffer.writeData(dataPtr);

	unmapBuffer(d3dbuffer);

	// Finally set the light constant buffer in the pixel shader with the updated values.
	setBuffer(d3dbuffer, buffer.layout.getShaderType(), 1, buffer.layout.getNumber());
}

void DXShader::updateBuffers(BufferUpdateType type)
{
	for (unsigned int i = 0; i < prtc_Buffers.size(); i++)
	{
		if (prtc_Buffers[i].layout.getUpdateType() == type || 
			type == BufferUpdateType::ALL  || 
			prtc_Buffers[i].layout.getUpdateType() == BufferUpdateType::ALL)
		{
			updateBuffer(prtc_Buffers[i]);
		}
	}
}

PLATFORM_NAMESPACE_END