#include "Shader.h"

#include "Triton/Logger/Log.h"

Triton::PType::Shader::Shader(ShaderLayout* layout)
{
	for (ShaderBufferLayout& bLayout : *layout)
	{
		auto* data = new unsigned char[bLayout.getStride()];
		prtc_Buffers.push_back(Buffer(bLayout, data));
	}
}

Triton::PType::Shader::~Shader()
{
	for (Buffer& buffer : prtc_Buffers)
	{
		delete[] buffer.data;
	}
}

void Triton::PType::Shader::setBufferValue(std::string bufferName, std::string elementName, void* value)
{
	for (unsigned int i = 0; i < prtc_Buffers.size(); i++)
	{
		if (prtc_Buffers[i].layout.getName() != bufferName)
		{
			continue;
		}

		prtc_Buffers[i].editVariable(elementName, (unsigned char*)value);
		return;
	}
}

void Triton::PType::Shader::setBufferValue(std::string bufferName, std::string elementName, Triton::Matrix44* value)
{
	auto transposed = DirectX::XMMatrixTranspose(*value);

	setBufferValue(bufferName, elementName, &transposed);
}

Triton::PType::ShaderBufferLayout::ShaderBufferLayout(std::string name, BufferUpdateType updateType, BufferShaderType shaderType, const std::initializer_list<ShaderVariable>& variables)
	: m_Elements(variables), m_Name(name), m_UpdateType(updateType), m_ShaderType(shaderType)
{
	uint32_t offset = 0;
	m_Stride = 0;
	for (auto& element : m_Elements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}

	// Make sure that the buffer is a multiple of 16 bytes
	unsigned int padding = m_Stride % 16;
	m_Stride += padding;
}

Triton::PType::ShaderBufferLayout::ShaderBufferLayout(std::string name, BufferUpdateType updateType, BufferShaderType shaderType, const std::vector<ShaderVariable>& variables)
	: m_Elements(variables), m_Name(name), m_UpdateType(updateType), m_ShaderType(shaderType)
{
	uint32_t offset = 0;
	m_Stride = 0;
	for (auto& element : m_Elements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}

	// Make sure that the buffer is a multiple of 16 bytes
	unsigned int padding = m_Stride % 16;
	m_Stride += padding;
}

Triton::PType::ShaderBufferLayout::~ShaderBufferLayout()
{
}

void Triton::PType::Shader::Buffer::writeData(unsigned char* dst)
{
	std::memcpy(dst, data, layout.getStride());
}

void Triton::PType::Shader::Buffer::editVariable(const std::string& variableName, unsigned char* variable)
{
	for (ShaderVariable& shaderVariable : layout)
	{
		if (shaderVariable.Name == variableName)
		{
			std::memcpy(data + shaderVariable.Offset, variable, shaderVariable.Size);
			return;
		}
	}
}

Triton::PType::ShaderLayout::ShaderLayout(const ShaderInputLayout& inputLayout, const std::initializer_list<ShaderBufferLayout>& variables)
	: m_Buffers(variables), m_InputLayout(inputLayout)
{
	std::unordered_map<BufferShaderType, unsigned int> numbers;

	for (ShaderBufferLayout& buffer : m_Buffers)
	{
		if (numbers.find(buffer.getShaderType()) == numbers.end())
		{
			numbers[buffer.getShaderType()] = 0;
		}
		else
		{
			numbers[buffer.getShaderType()]++;
		}

		buffer.setNumber(numbers[buffer.getShaderType()]);
	}
}

Triton::PType::ShaderLayout::ShaderLayout(const ShaderInputLayout & inputLayout, const std::vector<ShaderBufferLayout>& variables)
	: m_Buffers(variables), m_InputLayout(inputLayout)
{
	std::unordered_map<BufferShaderType, unsigned int> numbers;

	for (ShaderBufferLayout& buffer : m_Buffers)
	{
		if (numbers.find(buffer.getShaderType()) == numbers.end())
		{
			numbers[buffer.getShaderType()] = 0;
		}
		else
		{
			numbers[buffer.getShaderType()]++;
		}

		buffer.setNumber(numbers[buffer.getShaderType()]);
	}
}

Triton::PType::ShaderInputLayout::ShaderInputLayout(std::string name, const std::initializer_list<ShaderInputVariable>& variables)
	: m_Name(name), m_Elements(variables)
{
}

Triton::PType::ShaderInputLayout::ShaderInputLayout(std::string name, const std::vector<ShaderInputVariable>& variables)
	: m_Name(name), m_Elements(variables)
{
}

uint32_t Triton::PType::ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return 4;
	case ShaderDataType::Float2:   return 4 * 2;
	case ShaderDataType::Float3:   return 4 * 3;
	case ShaderDataType::Float4:   return 4 * 4;
	case ShaderDataType::Mat4:     return 4 * 4 * 4;
	case ShaderDataType::Int:      return 4;
	case ShaderDataType::Int2:     return 4 * 2;
	case ShaderDataType::Int3:     return 4 * 3;
	case ShaderDataType::Int4:     return 4 * 4;
	case ShaderDataType::Bool:     return 1;
	}

	TR_ERROR("Unknown shader type!");
	return 0;
}