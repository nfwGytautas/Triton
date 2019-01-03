#include "TRpch.h"
#include "WindowsModel.h"

#include <GL\glew.h>
#include "Triton\Core\Data\Storage\MeshStorage.h"

namespace Triton
{
	std::shared_ptr<Data::Mesh> Data::Mesh::Create(Storage::MeshStorage& aStorage, std::vector<float>& aVertices, std::vector<float>& aColorData, bool a3D)
	{
		return aStorage.Store(new Core::WindowsMesh(aVertices, aColorData, a3D));
	}

	namespace Core
	{		
		WindowsMesh::WindowsMesh(std::vector<float>& aVertices, std::vector<float>& aColorData, bool a3D)
		{
			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			m_VAO = vao;

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			m_VBOS.push_back(vbo);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, aVertices.size() * sizeof(float), &aVertices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, (a3D ? 3 : 2), GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(0);

			unsigned int vbo2;
			glGenBuffers(1, &vbo2);
			m_VBOS.push_back(vbo2);

			glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			glBufferData(GL_ARRAY_BUFFER, aColorData.size() * sizeof(float), &aColorData[0], GL_STATIC_DRAW);

			glVertexAttribPointer(1, (a3D ? 3 : 2), GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(1);

			m_IndiceCount = aVertices.size() / (a3D ? 3 : 2);
		}

		WindowsMesh::~WindowsMesh()
		{
			glDeleteVertexArrays(1, &m_VAO);
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