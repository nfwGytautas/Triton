#pragma once

#include <d3dcompiler.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN


inline void Triton::PType::DXShader::setUniformInt(const std::string& aUniformName, const int& aParameter)
{
	
}

inline void Triton::PType::DXShader::setUniformFloat(const std::string& aUniformName, const float& aParameter)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* dataPtr;
	unsigned int bufferNumber;

	// Lock the light constant buffer so it can be written to.
	result = m_deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	updateCBufferFloat(dataPtr, aUniformName, aParameter);

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	result = m_deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}

	// Get a pointer to the data in the constant buffer.
	updateCBufferFloat(dataPtr, aUniformName, aParameter);

	// Unlock the camera constant buffer.
	m_deviceContext->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
}

inline void Triton::PType::DXShader::setUniformVector2(const std::string& aUniformName, const Vector2 aParameter)
{
	
}

inline void Triton::PType::DXShader::setUniformVector3(const std::string& aUniformName, const Vector3 aParameter)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* dataPtr;
	unsigned int bufferNumber;

	// Lock the light constant buffer so it can be written to.
	result = m_deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	updateCBufferVector3(dataPtr, aUniformName, aParameter);

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	result = m_deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}

	// Get a pointer to the data in the constant buffer.
	updateCBufferVector3(dataPtr, aUniformName, aParameter);

	// Unlock the camera constant buffer.
	m_deviceContext->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
}

inline void Triton::PType::DXShader::setUniformVector4(const std::string& aUniformName, const Vector4 aParameter)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* dataPtr;
	unsigned int bufferNumber;

	// Lock the light constant buffer so it can be written to.
	result = m_deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	updateCBufferVector4(dataPtr, aUniformName, aParameter);

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);


	result = m_deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return;
	}

	// Get a pointer to the data in the constant buffer.
	updateCBufferVector4(dataPtr, aUniformName, aParameter);

	// Unlock the camera constant buffer.
	m_deviceContext->Unmap(m_cameraBuffer, 0);


	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
}

inline void Triton::PType::DXShader::setUniformMatrix44(const std::string& aUniformName, const Matrix44& aParameter)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	auto transposed = DirectX::XMMatrixTranspose(aParameter);

	// Lock the constant buffer so it can be written to.
	result = m_deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		TR_ERROR("Locking of constant buffer failed!");
		return ;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	updateCBufferMatrix(dataPtr, aUniformName, transposed);

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
}

inline void Triton::PType::DXShader::enable()
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

inline void Triton::PType::DXShader::disable()
{
	
	enabled = false;
}

inline void Triton::PType::DXShader::create(FactoryCreateParams* createParams)
{
	//TR_CORE_INFO("Loading shader complete. ID: {0}", m_shaderID);
}

inline void Triton::PType::DXShader::destroy(FactoryDestroyParams* destroyParams)
{
	// Release the camera constant buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
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

	return;
}

inline void DXShader::updateCBufferMatrix(MatrixBufferType* buffer, const std::string& name, const Matrix44& matrix)
{
	if (name == "transformationMatrix")
	{
		buffer->worldMatrix = matrix;
		m_prevMatrixVal.worldMatrix = matrix;
		buffer->projectionMatrix = m_prevMatrixVal.projectionMatrix;
		buffer->viewMatrix = m_prevMatrixVal.viewMatrix;
		return;
	}

	if (name == "viewMatrix")
	{
		buffer->viewMatrix = matrix;
		m_prevMatrixVal.viewMatrix = matrix;
		buffer->worldMatrix = m_prevMatrixVal.worldMatrix;
		buffer->projectionMatrix = m_prevMatrixVal.projectionMatrix;
		return;
	}

	if (name == "projectionMatrix")
	{
		buffer->projectionMatrix = matrix;
		m_prevMatrixVal.projectionMatrix = matrix;
		buffer->worldMatrix = m_prevMatrixVal.worldMatrix;
		buffer->viewMatrix = m_prevMatrixVal.viewMatrix;
		return;
	}

}

inline void Triton::PType::DXShader::updateCBufferFloat(void* buffer, const std::string & name, const float & vector)
{
	if (name == "specularPower")
	{
		((LightBufferType*)buffer)->specularPower = vector;
		m_prevLightVal.specularPower = vector;
		((LightBufferType*)buffer)->ambientColor = m_prevLightVal.ambientColor;
		((LightBufferType*)buffer)->diffuseColor = m_prevLightVal.diffuseColor;
		((LightBufferType*)buffer)->specularColor = m_prevLightVal.specularColor;
		((LightBufferType*)buffer)->lightDirection = m_prevLightVal.lightDirection;
		return;
	}
}

inline void Triton::PType::DXShader::updateCBufferVector3(void* buffer, const std::string& name, const Vector3& vector)
{
	if (name == "lightDirection")
	{
		((LightBufferType*)buffer)->lightDirection = vector;
		m_prevLightVal.lightDirection = vector;
		((LightBufferType*)buffer)->ambientColor = m_prevLightVal.ambientColor;
		((LightBufferType*)buffer)->diffuseColor = m_prevLightVal.diffuseColor;
		((LightBufferType*)buffer)->specularColor = m_prevLightVal.specularColor;
		((LightBufferType*)buffer)->specularPower = m_prevLightVal.specularPower;
		return;
	}

	if (name == "cameraPosition")
	{
		((CameraBufferType*)buffer)->cameraPosition = vector;
		m_prevCameraVal.cameraPosition = vector;
		((CameraBufferType*)buffer)->padding = 0;
		m_prevCameraVal.padding = 0;
		return;
	}
}

inline void Triton::PType::DXShader::updateCBufferVector4(void* buffer, const std::string& name, const Vector4& vector)
{
	if (name == "diffuseColor")
	{	
		((LightBufferType*)buffer)->diffuseColor = vector;
		m_prevLightVal.diffuseColor = vector;
		((LightBufferType*)buffer)->ambientColor = m_prevLightVal.ambientColor;
		((LightBufferType*)buffer)->lightDirection = m_prevLightVal.lightDirection;
		((LightBufferType*)buffer)->specularColor = m_prevLightVal.specularColor;
		((LightBufferType*)buffer)->specularPower = m_prevLightVal.specularPower;
		return;
	}

	if (name == "ambientColor")
	{
		((LightBufferType*)buffer)->ambientColor = vector;
		m_prevLightVal.ambientColor = vector;
		((LightBufferType*)buffer)->diffuseColor = m_prevLightVal.diffuseColor;
		((LightBufferType*)buffer)->lightDirection = m_prevLightVal.lightDirection;
		((LightBufferType*)buffer)->specularColor = m_prevLightVal.specularColor;
		((LightBufferType*)buffer)->specularPower = m_prevLightVal.specularPower;
		return;
	}

	if (name == "specularColor")
	{
		((LightBufferType*)buffer)->specularColor = vector;
		m_prevLightVal.specularColor = vector;
		((LightBufferType*)buffer)->ambientColor = m_prevLightVal.ambientColor;
		((LightBufferType*)buffer)->lightDirection = m_prevLightVal.lightDirection;
		((LightBufferType*)buffer)->specularColor = m_prevLightVal.specularColor;
		((LightBufferType*)buffer)->specularPower = m_prevLightVal.specularPower;
		return;
	}
}

NAMESPACE_END