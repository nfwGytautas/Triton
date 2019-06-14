#pragma once

#include "TritonPlatform/mathematical.h"
#include "Factory.h"
#include "Triton/Logger/Log.h"

namespace Triton
{
	namespace PType
	{
		class ShaderLayout;

		enum class BufferUpdateType
		{
			// Instruct the shader to update all buffers
			ALL = 0,

			// Instruct the shader to update only those buffers whose update type is frame e.g. View matrix
			FRAME = 1,

			// Instruct the shader to update only those buffers who should be updated for each object in the game e.g. Transformation matrix
			OBJECT = 2,

			// Instruct the shader to update only those buffers who should be updated rarely e.g. Projection matrix
			PERSISTANT = 3
		};

		enum class BufferShaderType
		{
			// Buffer is part of the pixel shader
			PIXEL = 0,

			// Buffer is part of the vertex shader
			VERTEX = 1
		};

		enum class ShaderDataType
		{
			None = 0, Float, Float2, Float3, Float4, Mat4, Int, Int2, Int3, Int4, Bool
		};

		static uint32_t ShaderDataTypeSize(ShaderDataType type)
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
		
			TR_CORE_ERROR("Unknown shader type!");
			return 0;
		}
		
		class ShaderCreateParams : public FactoryCreateParams
		{
		public:
			std::string vertexPath;
			std::string fragmentPath;

			ShaderLayout* layout;
		};

		struct ShaderVariable
		{
			ShaderVariable(std::string name, ShaderDataType type)
				: Name(name), Size(ShaderDataTypeSize(type))
			{

			}

			std::string Name;

			uint32_t Offset;
			uint32_t Size;
		};

		class ShaderBufferLayout
		{
		public:
			ShaderBufferLayout(std::string name, BufferUpdateType updateType, BufferShaderType shaderType, const std::initializer_list<ShaderVariable>& variables);

			~ShaderBufferLayout();

			inline uint32_t getStride() const { return m_Stride; }
			inline uint32_t getNumber() const { return m_Number; }
			inline void setNumber(uint32_t number) { m_Number = number; }
			inline const BufferUpdateType& getUpdateType() const { return m_UpdateType; }
			inline const BufferShaderType& getShaderType() const { return m_ShaderType; }
			inline const std::string& getName() const { return m_Name; }

			inline const std::vector<ShaderVariable>& getVariables() const { return m_Elements; }

			std::vector<ShaderVariable>::iterator begin() { return m_Elements.begin(); }
			std::vector<ShaderVariable>::iterator end() { return m_Elements.end(); }
			std::vector<ShaderVariable>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<ShaderVariable>::const_iterator end() const { return m_Elements.end(); }
		private:
			std::string m_Name;
			std::vector<ShaderVariable> m_Elements;

			BufferUpdateType m_UpdateType;
			BufferShaderType m_ShaderType;
			uint32_t m_Number;
			uint32_t m_Stride = 0;
		};

		struct ShaderInputVariable
		{
			ShaderInputVariable(std::string name, ShaderDataType type)
				: Name(name), Type(type)
			{

			}

			std::string Name;
			ShaderDataType Type;
		};

		class ShaderInputLayout
		{
		public:
			ShaderInputLayout() {}
			~ShaderInputLayout() {}

			ShaderInputLayout(const std::initializer_list<ShaderInputVariable>& variables);

			inline const std::vector<ShaderInputVariable>& getElements() const { return m_Elements; }
			inline unsigned int getVariableCount() const { return m_Elements.size(); }

			std::vector<ShaderInputVariable>::iterator begin() { return m_Elements.begin(); }
			std::vector<ShaderInputVariable>::iterator end() { return m_Elements.end(); }
			std::vector<ShaderInputVariable>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<ShaderInputVariable>::const_iterator end() const { return m_Elements.end(); }
		private:
			std::vector<ShaderInputVariable> m_Elements;
		};

		class ShaderLayout
		{
		public:
			ShaderLayout() {}
			~ShaderLayout() {}

			ShaderLayout(const ShaderInputLayout& inputLayout, const std::initializer_list<ShaderBufferLayout>& variables);

			inline const std::vector<ShaderBufferLayout>& getBuffers() const { return m_Buffers; }
			inline const ShaderInputLayout& getInputLayout() const { return m_InputLayout; }

			std::vector<ShaderBufferLayout>::iterator begin() { return m_Buffers.begin(); }
			std::vector<ShaderBufferLayout>::iterator end() { return m_Buffers.end(); }
			std::vector<ShaderBufferLayout>::const_iterator begin() const { return m_Buffers.begin(); }
			std::vector<ShaderBufferLayout>::const_iterator end() const { return m_Buffers.end(); }
		private:
			ShaderInputLayout m_InputLayout;
			std::vector<ShaderBufferLayout> m_Buffers;
		};

		class Shader : public FactoryObject
		{
		public:
			Shader(ShaderLayout* layout);
			virtual ~Shader();

			// Platform specific methods
			virtual void updateBuffers(BufferUpdateType type) = 0;
			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual void setBufferValue(std::string bufferName, std::string elementName, void* value);

			// Additional method for setting matrices since they need to be transposed before being set in the shader
			virtual void setBufferValue(std::string bufferName, std::string elementName, Triton::Matrix44* value);

		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
			virtual void destroy(FactoryDestroyParams* destroyParams) override = 0;
		protected:
			Shader()
			{ }

		protected:
			bool enabled = false;

			class Buffer
			{
			public:
				ShaderBufferLayout layout;
				unsigned char* data;

				Buffer(ShaderBufferLayout layout, unsigned char* data)
					: layout(layout), data(data)
				{

				}

				void writeData(unsigned char* dst);
				void editVariable(const std::string& variableName, unsigned char* variable);
			};
			std::vector<Buffer> prtc_Buffers;
		};
	}
}