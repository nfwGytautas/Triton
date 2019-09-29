#include "DXShader.h"

#include "Triton2/Utility/Log.h"

PLATFORM_NAMESPACE_BEGIN

DXShader::DXShader(const Flags::ShaderFlagset& flags)
	: Shader(flags)
{
}

DXShader::~DXShader()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_settingsBuffer)
	{
		m_settingsBuffer->Release();
		m_settingsBuffer = 0;
	}

	if (m_plightBuffer)
	{
		m_plightBuffer->Release();
		m_plightBuffer = 0;
	}

	if (m_slightBuffer)
	{
		m_slightBuffer->Release();
		m_slightBuffer = 0;
	}

	if (m_dlightBuffer)
	{
		m_dlightBuffer->Release();
		m_dlightBuffer = 0;
	}

	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
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

void DXShader::updateSettingsBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SettingsBuffer* dataPtr;

	mapBuffer(m_settingsBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<SettingsBuffer*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	*dataPtr = prtc_Settings;

	unmapBuffer(m_settingsBuffer);

	m_deviceContext->VSSetConstantBuffers(c_SettingsBufferSlot, 1, &m_settingsBuffer);
	m_deviceContext->PSSetConstantBuffers(c_SettingsBufferSlot, 1, &m_settingsBuffer);
}

void DXShader::updateMatricesBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;

	mapBuffer(m_matrixBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<MatrixBuffer*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	*dataPtr = prtc_Matrices;

	unmapBuffer(m_matrixBuffer);

	m_deviceContext->VSSetConstantBuffers(c_MatrixBufferSlot, 1, &m_matrixBuffer);
	m_deviceContext->PSSetConstantBuffers(c_MatrixBufferSlot, 1, &m_matrixBuffer);
}

void DXShader::updateLightingBuffer()
{
	updatePLightBuffer();
	updateSLightBuffer();
	updateDLightBuffer();
}

void DXShader::updatePLightBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PointLights* dataPtr;

	mapBuffer(m_plightBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<PointLights*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	*dataPtr = prtc_PLights;

	unmapBuffer(m_plightBuffer);

	m_deviceContext->VSSetConstantBuffers(c_PointLightsSlot, 1, &m_plightBuffer);
	m_deviceContext->PSSetConstantBuffers(c_PointLightsSlot, 1, &m_plightBuffer);
}

void DXShader::updateSLightBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SpotLights* dataPtr;

	mapBuffer(m_slightBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<SpotLights*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	*dataPtr = prtc_SLights;

	unmapBuffer(m_slightBuffer);

	m_deviceContext->VSSetConstantBuffers(c_SpotLightsSlot, 1, &m_slightBuffer);
	m_deviceContext->PSSetConstantBuffers(c_SpotLightsSlot, 1, &m_slightBuffer);
}

void DXShader::updateDLightBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DirectionalLights* dataPtr;

	mapBuffer(m_dlightBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<DirectionalLights*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.

	*dataPtr = prtc_DLights;

	unmapBuffer(m_dlightBuffer);

	m_deviceContext->VSSetConstantBuffers(c_DirectionalLightsSlot, 1, &m_dlightBuffer);
	m_deviceContext->PSSetConstantBuffers(c_DirectionalLightsSlot, 1, &m_dlightBuffer);
}

void DXShader::updateCameraBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CameraBuffer* dataPtr;

	mapBuffer(m_cameraBuffer, mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<CameraBuffer*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	*dataPtr = prtc_Camera;

	unmapBuffer(m_cameraBuffer);

	m_deviceContext->VSSetConstantBuffers(c_CameraSlot, 1, &m_cameraBuffer);
	m_deviceContext->PSSetConstantBuffers(c_CameraSlot, 1, &m_cameraBuffer);
}

void DXShader::update_matrices()
{
	if (m_recalculateLightBuffer)
	{
		update_lights();
	}

	// Settings buffer is updated no matter what
	if (m_flags.test(Flags::sFlag_Settings))
	{
		updateSettingsBuffer();
	}

	if (m_flags.test(Flags::sFlag_Matrices))
	{
		updateMatricesBuffer();
	}

	if (m_flags.test(Flags::sFlag_Lighting))
	{
		updateLightingBuffer();
	}

	if (m_flags.test(Flags::sFlag_Camera))
	{
		updateCameraBuffer();
	}
}

PLATFORM_NAMESPACE_END