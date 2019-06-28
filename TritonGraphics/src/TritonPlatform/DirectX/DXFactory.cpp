#include "DXFactory.h"

#include "comdef.h"
#include <d3dcompiler.h>
#include "DXManip.h"

#include "BasicTypes\DXShader.h"
#include "BasicTypes\DXTexture.h"
#include "BasicTypes\DXVAO.h"
#include "BasicTypes\DXFramebuffer.h"
#include "BasicTypes\DXBitmap.h"

PLATFORM_NAMESPACE_BEGIN

reference<FactoryObject> Triton::PType::DXFactory::createShader(FactoryCreateParams* createParams)
{
	auto shaderCreate = OBJECT_AS(ShaderCreateParams, createParams);

	auto layout = shaderCreate->layout;

	DXShader* shader = new DXShader(layout);

	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	D3D11_SAMPLER_DESC samplerDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	auto vsFilename = Impl::s2ws(shaderCreate->vertexPath);
	result = D3DCompileFromFile(vsFilename.c_str(), NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, &vsFilename[0]);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, vsFilename.c_str(), L"Missing Shader File", MB_OK);
		}

		return nullptr;
	}

	// Compile the pixel shader code.
	auto psFilename = Impl::s2ws(shaderCreate->fragmentPath);
	result = D3DCompileFromFile(psFilename.c_str(), NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, &psFilename[0]);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(m_hwnd, psFilename.c_str(), L"Missing Shader File", MB_OK);
		}

		return nullptr;
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &shader->m_vertexShader);
	if (FAILED(result))
	{
		TR_ERROR("Creating vertex shader from buffer failed");
		return nullptr;
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &shader->m_pixelShader);
	if (FAILED(result))
	{
		TR_ERROR("Creating pixel shader from buffer failed");
		return false;
	}


	inputDesc.reserve(layout->getInputLayout().getVariableCount());
	// Create the vertex input layout description.
	for (const ShaderInputVariable& siVariable : layout->getInputLayout())
	{
		D3D11_INPUT_ELEMENT_DESC polygonLayout;

		polygonLayout.SemanticName = siVariable.Name.c_str();
		polygonLayout.SemanticIndex = 0;
		polygonLayout.Format = Impl::sdtToDXGIFormat(siVariable.Type);
		polygonLayout.InputSlot = 0;
		polygonLayout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout.InstanceDataStepRate = 0;

		inputDesc.push_back(polygonLayout);
	}

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(&inputDesc[0], inputDesc.size(), vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &shader->m_layout);
	if (FAILED(result))
	{
		_com_error err(result);
		LPCTSTR errMsg = err.ErrorMessage();
		TR_CORE_ERROR("Creating vertex input layout failed");
		return nullptr;
	}


	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	for (ShaderBufferLayout& buffer : *shaderCreate->layout)
	{
		D3D11_BUFFER_DESC bufferDesc;

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = buffer.getStride();
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = m_device->CreateBuffer(&bufferDesc, NULL, &shader->m_buffers[buffer.getName()]);
		if (FAILED(result))
		{
			TR_ERROR("Creating constant buffer pointer failed");
			return nullptr;
		}
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = m_device->CreateSamplerState(&samplerDesc, &shader->m_sampleState);
	if (FAILED(result))
	{
		TR_ERROR("Creating texture sampler state failed");
		return nullptr;
	}

	shader->m_deviceContext = m_deviceContext;
	shader->create(createParams);
	return shader;
}

reference<FactoryObject> Triton::PType::DXFactory::createVAO(FactoryCreateParams* createParams)
{
	DXVAO* vao = new DXVAO();

	VAOCreateParams::Vertex* vertices;
	unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	auto vaoParams = OBJECT_AS(VAOCreateParams, createParams);

	// Set the number of vertices in the vertex array.
	unsigned int vertexCount = vaoParams->vertices.size();

	// Set the number of indices in the index array.
	unsigned int indexCount = vaoParams->indices.size();

	// Create the vertex array.
	vertices = vaoParams->vertices.data();
	if (!vertices)
	{
		TR_ERROR("Creating vertex array failed");
		return nullptr;
	}

	// Create the index array.
	indices = vaoParams->indices.data();
	if (!indices)
	{
		TR_ERROR("Creating index array failed");
		return nullptr;
	}

	vao->m_indiceCount = indexCount;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VAOCreateParams::Vertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &vao->m_vertexBuffer);
	if (FAILED(result))
	{
		TR_ERROR("Creating vertex buffer failed");
		return nullptr;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &vao->m_indexBuffer);
	if (FAILED(result))
	{
		TR_ERROR("Creating index buffer failed");
		return nullptr;
	}

	vao->m_deviceContext = m_deviceContext;
	vao->create(createParams);
	return vao;
}

reference<FactoryObject> Triton::PType::DXFactory::createTexture(FactoryCreateParams* createParams)
{
	DXTexture* texture = new DXTexture();

	auto textureParams = OBJECT_AS(TextureCreateParams, createParams);

	bool result;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Setup the description of the texture.
	textureDesc.Height = textureParams->height;
	textureDesc.Width = textureParams->width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = m_device->CreateTexture2D(&textureDesc, NULL, &texture->m_texture);
	if (FAILED(hResult))
	{
		TR_ERROR("Creating empty texture failed");
		return nullptr;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (textureParams->width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	m_deviceContext->UpdateSubresource(texture->m_texture, 0, NULL, textureParams->buffer.get(), rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = m_device->CreateShaderResourceView(texture->m_texture, &srvDesc, &texture->m_textureView);
	if (FAILED(hResult))
	{
		TR_ERROR("Creating shader resource view failed");
		return nullptr;
	}

	// Generate mipmaps for this texture.
	m_deviceContext->GenerateMips(texture->m_textureView);


	texture->m_deviceContext = m_deviceContext;

	texture->create(createParams);
	return texture;
}

reference<FactoryObject> DXFactory::createFramebuffer(FactoryCreateParams* createParams)
{
	DXFramebuffer* frameBuffer = new DXFramebuffer();

	auto textureParams = OBJECT_AS(TextureCreateParams, createParams);

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureParams->width;
	textureDesc.Height = textureParams->height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = m_device->CreateTexture2D(&textureDesc, NULL, &frameBuffer->m_renderTargetTexture);
	if (FAILED(result))
	{
		return nullptr;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = m_device->CreateRenderTargetView(frameBuffer->m_renderTargetTexture, &renderTargetViewDesc, &frameBuffer->m_renderTargetView);
	if (FAILED(result))
	{
		return nullptr;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = m_device->CreateShaderResourceView(frameBuffer->m_renderTargetTexture, &shaderResourceViewDesc, &frameBuffer->m_shaderResourceView);
	if (FAILED(result))
	{
		return nullptr;
	}

	frameBuffer->m_deviceContext = m_deviceContext;
	frameBuffer->m_depthStencilView = m_depthStencilView;

	return frameBuffer;
}

reference<FactoryObject> Triton::PType::DXFactory::createBitmap(FactoryCreateParams* createParams)
{
	DXBitmap* bitmap = new DXBitmap();

	auto bitmapParams = OBJECT_AS(BitmapCreateParams, createParams);

	// Temp
	RECT rcClient;
	GetClientRect(m_hwnd, &rcClient);
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	bitmap->prtc_screenWidth = 250;
	bitmap->prtc_screenHeight = 250;

	bitmap->prtc_bitmapWidth = bitmapParams->width;
	bitmap->prtc_bitmapHeight = bitmapParams->height;


	BitmapVertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Set the number of vertices in the vertex array.
	bitmap->m_vertexCount = 6;

	// Set the number of indices in the index array.
	bitmap->m_indiceCount = bitmap->m_vertexCount;

	// Create the vertex array.
	vertices = new BitmapVertexType[bitmap->m_vertexCount];

	// Create the index array.
	indices = new unsigned long[bitmap->m_indiceCount];
	if (!indices)
	{
		TR_CORE_ERROR("Failed to create bitmap indice array");
		return nullptr;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(BitmapVertexType) * bitmap->m_vertexCount));

	// Load the index array with data.
	for (i = 0; i < bitmap->m_indiceCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(BitmapVertexType) * bitmap->m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &bitmap->m_vertexBuffer);
	if (FAILED(result))
	{
		TR_CORE_ERROR("Failed to create bitmap vertex buffer");
		return nullptr;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * bitmap->m_indiceCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &bitmap->m_indexBuffer);
	if (FAILED(result))
	{
		TR_CORE_ERROR("Failed to create bitmap indice buffer");
		return nullptr;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	// Create the texture object.
	bitmap->m_Texture = bitmapParams->texture;
	bitmap->m_deviceContext = m_deviceContext;

	bitmap->create(createParams);
	return bitmap;
}

void Triton::PType::DXFactory::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");
	std::stringstream ss;

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
		ss << compileErrors[i];
	}

	// Close the file.
	fout.close();

	TR_ERROR("Shader compiling error: {0}", ss.str());

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

PLATFORM_NAMESPACE_END