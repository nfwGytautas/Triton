#include "DXContext.h"

#include <sstream>

#include "comdef.h"
#include <d3dcompiler.h>

#include "DXWindow.h"
#include "DXRenderer.h"

#include "TritonPlatform2/DirectX/BasicTypes/DXShader.h"
#include "TritonPlatform2/DirectX/BasicTypes/DXVAO.h"
#include "TritonPlatform2/DirectX/BasicTypes/DXTexture.h"
#include "TritonPlatform2/DirectX/BasicTypes/DXFramebuffer.h"
#include "TritonPlatform2/DirectX/BasicTypes/DXBitmap.h"

#include "Triton2/Utility/Log.h"

std::wstring s2ws(const std::string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

namespace Triton
{
	namespace Graphics
	{
		void outputShaderErrorMessage(ID3D10Blob* errorMessage)
		{
			char* compileErrors;
			unsigned long long bufferSize, i;

			// Get a pointer to the error message text buffer.
			compileErrors = (char*)(errorMessage->GetBufferPointer());

			// Get the length of the message.
			bufferSize = errorMessage->GetBufferSize();

			std::stringstream ss;

			// Write out the error message.
			for (i = 0; i < bufferSize; i++)
			{
				ss << compileErrors[i];
			}

			TR_ERROR("Shader compiling error: {0}", ss.str());
			TR_SYSTEM_ERROR("Shader compiling error: {0}", ss.str());

			return;
		}

		bool DXContext::s_descsCreated = false;

		D3D11_BUFFER_DESC DXContext::s_matrixBufferDesc;
		D3D11_BUFFER_DESC DXContext::s_settingsBufferDesc;
		D3D11_BUFFER_DESC DXContext::s_plightBufferDesc;
		D3D11_BUFFER_DESC DXContext::s_slightBufferDesc;
		D3D11_BUFFER_DESC DXContext::s_dlightBufferDesc;
		D3D11_BUFFER_DESC DXContext::s_camBufferDesc;
		D3D11_SAMPLER_DESC DXContext::s_samplerDesc;

		void DXContext::createDescs()
		{
			// Matrix buffer
			TR_SYSTEM_TRACE("Creating shader matrix buffer description");
			s_matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			s_matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
			s_matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			s_matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			s_matrixBufferDesc.MiscFlags = 0;
			s_matrixBufferDesc.StructureByteStride = 0;

			// Settings buffer
			TR_SYSTEM_TRACE("Creating shader settings buffer description");
			s_settingsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			s_settingsBufferDesc.ByteWidth = sizeof(SettingsBuffer);
			s_settingsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			s_settingsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			s_settingsBufferDesc.MiscFlags = 0;
			s_settingsBufferDesc.StructureByteStride = 0;

			// Point light buffer
			TR_SYSTEM_TRACE("Creating shader point light buffer description");
			s_plightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			s_plightBufferDesc.ByteWidth = sizeof(PointLights);
			s_plightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			s_plightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			s_plightBufferDesc.MiscFlags = 0;
			s_plightBufferDesc.StructureByteStride = 0;

			// Settings buffer
			TR_SYSTEM_TRACE("Creating shader spot light buffer description");
			s_slightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			s_slightBufferDesc.ByteWidth = sizeof(SpotLights);
			s_slightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			s_slightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			s_slightBufferDesc.MiscFlags = 0;
			s_slightBufferDesc.StructureByteStride = 0;

			// Settings buffer
			TR_SYSTEM_TRACE("Creating shader directional light buffer description");
			s_dlightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			s_dlightBufferDesc.ByteWidth = sizeof(DirectionalLights);
			s_dlightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			s_dlightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			s_dlightBufferDesc.MiscFlags = 0;
			s_dlightBufferDesc.StructureByteStride = 0;

			// Camera buffer
			TR_SYSTEM_TRACE("Creating shader camera buffer description");
			s_camBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			s_camBufferDesc.ByteWidth = sizeof(CameraBuffer);
			s_camBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			s_camBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			s_camBufferDesc.MiscFlags = 0;
			s_camBufferDesc.StructureByteStride = 0;

			// Create a texture sampler state description.
			TR_SYSTEM_TRACE("Creating shader sampler description");
			s_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			s_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			s_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			s_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			s_samplerDesc.MipLODBias = 0.0f;
			s_samplerDesc.MaxAnisotropy = 1;
			s_samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			s_samplerDesc.BorderColor[0] = 0;
			s_samplerDesc.BorderColor[1] = 0;
			s_samplerDesc.BorderColor[2] = 0;
			s_samplerDesc.BorderColor[3] = 0;
			s_samplerDesc.MinLOD = 0;
			s_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			s_descsCreated = true;
		}

		DXContext::DXContext()
		{
		}

		DXContext::~DXContext()
		{
		}

		Window* DXContext::newWindow()
		{
			TR_SYSTEM_TRACE("Creating a new 'Windows' window");
			if (!m_initialized)
			{
				TR_SYSTEM_ERROR("Creating window before initializing the context!");
				return nullptr;
			}

			return static_cast<Window*>(new DXWindow(m_keyboardState, m_mouseState));
		}

		Renderer* DXContext::newRenderer(Window* window)
		{
			TR_SYSTEM_TRACE("Creating a new 'Windows' renderer");
			if (!m_initialized)
			{
				TR_SYSTEM_ERROR("Creating renderer before initializing the context!");
				return nullptr;
			}

			DXWindow* dxwindow = static_cast<DXWindow*>(window);
			return static_cast<Renderer*>(new DXRenderer(m_device, m_deviceContext, m_displayModeList, dxwindow));
		}

		bool DXContext::init()
		{
			TR_SYSTEM_TRACE("Initializing DXContext");

			HRESULT result;

			// First initialize DirectX 11

			// Set feature level to DX11
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

			TR_SYSTEM_TRACE("Creating DX device with feature level '{0}'", featureLevel);
			result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
				D3D11_SDK_VERSION, &m_device, NULL, &m_deviceContext);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create the DirectX 11 Device");
				return false;
			}
			TR_SYSTEM_INFO("DX device created");

			// Secondly lets query some information about the system and monitor
			IDXGIFactory* factory;
			IDXGIAdapter* adapter;
			DXGI_ADAPTER_DESC adapterDesc;
			int error;
			IDXGIOutput* adapterOutput;
			unsigned long long stringLength;
			unsigned int numModes;

			TR_SYSTEM_TRACE("Getting display parameters");
			// Lets create a factory
			result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create DX factory!");
				return false;
			}

			// Use the factory to create an adapter for the primary graphics interface (video card).
			result = factory->EnumAdapters(0, &adapter);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get adapters!");
				return false;
			}

			// Enumerate the primary adapter output (monitor).
			result = adapter->EnumOutputs(0, &adapterOutput);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to enumerate over the primary adapter!");
				return false;
			}

			TR_SYSTEM_TRACE("Adapter information acquired");

			// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
			result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to get the number of modes with 'DXGI_FORMAT_R8G8B8A8_UNORM' display format!");
				return false;
			}

			TR_SYSTEM_TRACE("Possible display mode count '{0}'", numModes);

			// Create a list to hold all the possible display modes for this monitor/video card combination.
			m_displayModeList.resize(numModes);

			// Get display mode list
			result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, m_displayModeList.data());
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to fill the display mode list!");
				return false;
			}

			TR_SYSTEM_TRACE("Getting video card information", numModes);

			// Get the adapter (video card) description.
			result = adapter->GetDesc(&adapterDesc);
			if (FAILED(result))
			{
				return false;
			}

			// Store the dedicated video card memory in megabytes.
			m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

			// Convert the name of the video card to a character array and store it.
			error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
			if (error != 0)
			{
				return false;
			}

			char* cardName = new char[128];
			strcpy_s(cardName, 128, m_videoCardDescription);
			TR_SYSTEM_INFO("Video card: '{0}' with {1}MB dedicated video memory", cardName, m_videoCardMemory);
			delete cardName;

			// Release the adapter output.
			adapterOutput->Release();

			// Release the adapter.
			adapter->Release();

			// Release the factory.
			factory->Release();

			m_keyboardState = new IO::Keyboard();
			m_mouseState = new IO::Mouse();
			
			m_initialized = true;

			TR_SYSTEM_TRACE("Checking context state descriptions");
			if (!s_descsCreated)
			{
				TR_SYSTEM_WARN("Context descriptions not created, creating now");
				createDescs();
			}
			else
			{
				TR_SYSTEM_TRACE("Descriptions already created");
			}

			return true;
		}

		void DXContext::shutdown()
		{
			TR_SYSTEM_WARN("Shutting down DXContext");

			if (m_deviceContext)
			{
				m_deviceContext->Release();
				m_deviceContext = 0;
			}

			if (m_device)
			{
				m_device->Release();
				m_device = 0;
			}

			delete m_keyboardState;
			delete m_mouseState;
		}

		Shader* DXContext::newShader(const IO::ShaderData& createParams)
		{
			TR_SYSTEM_TRACE("Creating direct x shader");

			DXShader* shader = new DXShader(createParams.flags);

			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;

			int descCount = 3;
			D3D11_INPUT_ELEMENT_DESC iaDescExtended[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			// Initialize the pointers this function will use to null.
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			TR_SYSTEM_TRACE("Compiling vertex shader");

			// Vertex shader
			result = D3DCompile(
				createParams.source.c_str(),
				createParams.source.length(),
				nullptr,
				nullptr,
				nullptr,
				createParams.vertexEntry.c_str(),
				"vs_5_0",
				D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
				0,
				&vertexShaderBuffer,
				&errorMessage);

			if (FAILED(result))
			{
				// If the shader failed to compile it should have written something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage);
				}
				// If there was  nothing in the error message then it simply could not find the shader file itself.
				else
				{
					TR_ERROR("Missing shader file");
				}

				return nullptr;
			}

			TR_SYSTEM_TRACE("Compiling pixel shader");

			// Pixel shader
			result = D3DCompile(
				createParams.source.c_str(),
				createParams.source.length(),
				nullptr,
				nullptr,
				nullptr,
				createParams.pixelEntry.c_str(),
				"ps_5_0",
				D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
				0,
				&pixelShaderBuffer,
				&errorMessage);

			if (FAILED(result))
			{
				// If the shader failed to compile it should have written something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage);
				}
				// If there was  nothing in the error message then it simply could not find the shader file itself.
				else
				{
					TR_ERROR("Missing shader file");
				}

				return nullptr;
			}

			TR_SYSTEM_TRACE("Creating shader");

			// Create the vertex shader from the buffer.
			result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &shader->m_vertexShader);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating vertex shader from buffer failed");
				return nullptr;
			}

			// Create the pixel shader from the buffer.
			result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &shader->m_pixelShader);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating pixel shader from buffer failed");
				return nullptr;
			}

			// Create the vertex input layout.
			result = m_device->CreateInputLayout(iaDescExtended, descCount, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &shader->m_layout);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating vertex input layout failed");
				return nullptr;
			}


			// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			pixelShaderBuffer->Release();
			pixelShaderBuffer = 0;

			TR_SYSTEM_TRACE("Creating shader buffers");

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = m_device->CreateBuffer(&s_matrixBufferDesc, NULL, &shader->m_matrixBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating matrix buffer failed");
				return nullptr;
			}

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = m_device->CreateBuffer(&s_settingsBufferDesc, NULL, &shader->m_settingsBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating settings buffer failed");
				return nullptr;
			}

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = m_device->CreateBuffer(&s_plightBufferDesc, NULL, &shader->m_plightBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating point light buffer failed");
				return nullptr;
			}

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = m_device->CreateBuffer(&s_slightBufferDesc, NULL, &shader->m_slightBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating spot light buffer failed");
				return nullptr;
			}

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = m_device->CreateBuffer(&s_dlightBufferDesc, NULL, &shader->m_dlightBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating directional light buffer failed");
				return nullptr;
			}

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = m_device->CreateBuffer(&s_camBufferDesc, NULL, &shader->m_cameraBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating camera buffer failed");
				return nullptr;
			}

			// Create the texture sampler state.
			result = m_device->CreateSamplerState(&s_samplerDesc, &shader->m_sampleState);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating texture sampler state failed");
				return nullptr;
			}

			shader->m_deviceContext = m_deviceContext;

			TR_SYSTEM_INFO("Shader created");

			return shader;
		}

		VAO* DXContext::newVAO(const IO::Mesh& createParams)
		{
			TR_SYSTEM_TRACE("Creating vertex array(VAO)");

			DXVAO* vao = new DXVAO();

			const IO::Vertex* vertices;
			const unsigned int* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			// Set the number of vertices in the vertex array.
			unsigned int vertexCount = (unsigned int)createParams.vertices.size();

			// Set the number of indices in the index array.
			unsigned int indexCount = (unsigned int)createParams.indices.size();

			TR_SYSTEM_TRACE("Vertices: '{0}' Indices: '{1}'", vertexCount, indexCount);

			// Create the vertex array.
			vertices = createParams.vertices.data();
			if (!vertices)
			{
				TR_SYSTEM_ERROR("Creating vertex array failed");
				return nullptr;
			}

			// Create the index array.
			indices = createParams.indices.data();
			if (!indices)
			{
				TR_SYSTEM_ERROR("Creating index array failed");
				return nullptr;
			}

			vao->m_indiceCount = indexCount;

			// Set up the description of the static vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(IO::Vertex) * vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the sub resource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			TR_SYSTEM_TRACE("Creating vertex array object(VAO) buffers");

			// Now create the vertex buffer.
			result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &vao->m_vertexBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating vertex buffer failed");
				return nullptr;
			}

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the sub resource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &vao->m_indexBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Creating index buffer failed");
				return nullptr;
			}

			vao->m_deviceContext = m_deviceContext;

			TR_SYSTEM_INFO("Vertex array object(VAO) created");

			return vao;
		}

		Texture* DXContext::newTexture(const IO::ImageData& createParams)
		{
			TR_SYSTEM_TRACE("Creating a texture");

			DXTexture* texture = new DXTexture();

			D3D11_TEXTURE2D_DESC textureDesc;
			HRESULT hResult;
			unsigned int rowPitch;
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			// Setup the description of the texture.
			textureDesc.Height = createParams.height;
			textureDesc.Width = createParams.width;
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
				TR_SYSTEM_ERROR("Creating empty texture failed");
				return nullptr;
			}

			// Set the row pitch of the targa image data.
			rowPitch = (createParams.width * 4) * sizeof(unsigned char);

			TR_SYSTEM_TRACE("Creating texture resources");

			// Copy the image data into the texture.
			m_deviceContext->UpdateSubresource(texture->m_texture, 0, NULL, createParams.buffer.get(), rowPitch, 0);

			// Setup the shader resource view description.
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = -1;

			// Create the shader resource view for the texture.
			hResult = m_device->CreateShaderResourceView(texture->m_texture, &srvDesc, &texture->m_textureView);
			if (FAILED(hResult))
			{
				TR_SYSTEM_ERROR("Creating shader resource view failed");
				return nullptr;
			}

			TR_SYSTEM_TRACE("Generating texture mipmaps");

			// Generate mipmaps for this texture.
			m_deviceContext->GenerateMips(texture->m_textureView);

			texture->m_deviceContext = m_deviceContext;

			TR_SYSTEM_INFO("Texture created");

			return texture;
		}

		CubeTexture* DXContext::newCubeTexture(const IO::ImageArrayData& createParams)
		{
			TR_SYSTEM_TRACE("Creating a cube texture");

			DXCubeTexture* cubeTexture = new DXCubeTexture();

			D3D11_TEXTURE2D_DESC textureDesc;
			HRESULT hResult;
			unsigned int rowPitch;
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			// Setup the description of the texture.
			textureDesc.Height = createParams.height;
			textureDesc.Width = createParams.width;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = createParams.count;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

			// Setup the shader resource view description.
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = textureDesc.MipLevels;
			srvDesc.TextureCube.MostDetailedMip = 0;

			rowPitch = (createParams.width * 4) * sizeof(unsigned char);

			D3D11_SUBRESOURCE_DATA pData[6];
			for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
			{
				//Pointer to the pixel data
				pData[cubeMapFaceIndex].pSysMem = createParams.individualTextures[cubeMapFaceIndex]->buffer.get();
				//Line width in bytes
				pData[cubeMapFaceIndex].SysMemPitch = rowPitch;
				// This is only used for 3d textures.
				pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
			}
			
			TR_SYSTEM_TRACE("Creating cube texture resources");

			//Create the Texture Resource
			hResult = m_device->CreateTexture2D(&textureDesc, &pData[0], &cubeTexture->m_texture);
			if (FAILED(hResult))
			{
				TR_SYSTEM_ERROR("Creating cube texture failed");
				return nullptr;
			}

			// Create the shader resource view for the texture.
			hResult = m_device->CreateShaderResourceView(cubeTexture->m_texture, &srvDesc, &cubeTexture->m_textureView);
			if (FAILED(hResult))
			{
				TR_SYSTEM_ERROR("Creating shader resource view failed");
				return nullptr;
			}

			TR_SYSTEM_TRACE("Generating mipmaps");

			// Generate mipmaps for this texture.
			m_deviceContext->GenerateMips(cubeTexture->m_textureView);

			cubeTexture->m_deviceContext = m_deviceContext;

			TR_SYSTEM_INFO("Cube texture created");

			return cubeTexture;
		}

		Framebuffer* DXContext::newFramebuffer(unsigned int width, unsigned int height)
		{
			TR_SYSTEM_TRACE("Creating frame buffer '{0}x{1}'", width, height);

			DXFramebuffer* frameBuffer = new DXFramebuffer();

			D3D11_TEXTURE2D_DESC textureDesc;
			HRESULT result;
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


			// Initialize the render target texture description.
			ZeroMemory(&textureDesc, sizeof(textureDesc));

			// Setup the render target texture description.
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			TR_SYSTEM_TRACE("Creating frame buffer resources");

			// Create the render target texture.
			result = m_device->CreateTexture2D(&textureDesc, NULL, &frameBuffer->m_renderTargetTexture);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create 2D texture");
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
				TR_SYSTEM_ERROR("Failed to create render target view");
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
				TR_SYSTEM_ERROR("Failed to create shader resource view");
				return nullptr;
			}

			TR_SYSTEM_TRACE("Creating frame buffer depth stencil");

			// Get the depth stencil
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

			// Initialize the depth stencil view.
			ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

			// Set up the depth stencil view description.
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			// Create the depth stencil view.
			result = m_device->CreateDepthStencilView(frameBuffer->m_renderTargetTexture, &depthStencilViewDesc, &frameBuffer->m_depthStencilView);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create depth stencil view");
				return nullptr;
			}

			frameBuffer->m_deviceContext = m_deviceContext;

			TR_SYSTEM_INFO("Frame buffer created");

			return frameBuffer;
		}

		Bitmap* DXContext::newBitmap(const BitmapCreateParams& createParams)
		{
			TR_SYSTEM_TRACE("Creating bitmap");

			DXBitmap* bitmap = new DXBitmap();
			
			bitmap->prtc_screenWidth = 250;
			bitmap->prtc_screenHeight = 250;

			bitmap->prtc_bitmapWidth = createParams.width;
			bitmap->prtc_bitmapHeight = createParams.height;


			BitmapVertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;
			unsigned int i;


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
				TR_SYSTEM_ERROR("Failed to create bitmap indices array");
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

			// Give the sub resource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			TR_SYSTEM_TRACE("Creating bitmap buffers");

			// Now create the vertex buffer.
			result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &bitmap->m_vertexBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create bitmap vertex buffer");
				return nullptr;
			}

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * bitmap->m_indiceCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the sub resource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &bitmap->m_indexBuffer);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create bitmap indices buffer");
				return nullptr;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete[] vertices;
			vertices = 0;

			delete[] indices;
			indices = 0;

			// Create the texture object.
			bitmap->m_Texture = createParams.texture;
			bitmap->m_deviceContext = m_deviceContext;

			TR_SYSTEM_INFO("Bitmap created");

			return bitmap;
		}
	}
}