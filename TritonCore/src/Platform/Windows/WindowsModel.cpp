#include "TRpch.h"
#include "WindowsModel.h"

#include "Triton\Core\Math\Math.h"
#include <glad\glad.h>

namespace Triton
{
	namespace Core
	{
		unsigned int CreateVAO()
		{
			unsigned int vao;
			glGenVertexArrays(1, &vao);
			return vao;
		}

		unsigned int CreateIBO(std::vector<unsigned int> aIndices)
		{
			unsigned int ebo;
			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndices.size() * sizeof(unsigned int),
				&aIndices[0], GL_STATIC_DRAW);

			return ebo;
		}

		template<class T>
		unsigned int StoreDataInVBO(std::vector<T> aData)
		{
			unsigned int vbo;
			glGenBuffers(1, &vbo);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, aData.size() * sizeof(T), &aData[0], GL_STATIC_DRAW);

			return vbo;
		}

		unsigned int storeDataInAttributes(unsigned int location, unsigned int dataSize, std::vector<float> data)
		{
			unsigned int vboID;

			glGenBuffers(1, &vboID);

			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

			glVertexAttribPointer(
				location,
				dataSize,
				GL_FLOAT,
				false,
				dataSize * sizeof(float),
				(const void*)0
			);

			glEnableVertexAttribArray(location);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			return vboID;
		}
	}
}

namespace Triton
{
	std::shared_ptr<Data::Mesh> Data::Mesh::Create(MeshData& aData)
	{
		return std::make_shared<Core::WindowsMesh>(aData);
	}

	namespace Core
	{		
		WindowsMesh::WindowsMesh(Data::MeshData& aData)
		{
			m_VAO = Triton::Core::CreateVAO();
			Bind();

			m_IBO = CreateIBO(aData.Indices);

			if(aData.Vertices.size() > 0)
				m_VBOS.push_back(storeDataInAttributes(0, 3, aData.Vertices));
			if (aData.UVS.size() > 0)
				m_VBOS.push_back(storeDataInAttributes(1, 2, aData.UVS));
			if (aData.Normals.size() > 0)
				m_VBOS.push_back(storeDataInAttributes(2, 3, aData.Normals));

			m_IndiceCount = aData.Indices.size();

			Unbind();
		}

		WindowsMesh::~WindowsMesh()
		{
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(0, &m_IBO);
			glDeleteBuffers(m_VBOS.size(), &m_VBOS[0]);
		}

		void WindowsMesh::Bind()
		{
			glBindVertexArray(m_VAO);
		}

		void WindowsMesh::Unbind()
		{
			glBindVertexArray(0);
		}

		unsigned int WindowsMesh::GetIndiceCount() const
		{
			return m_IndiceCount;
		}
	}
}