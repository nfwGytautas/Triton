#include "TRpch.h"
#include "WindowsModel.h"

#include <GL\glew.h>

namespace Triton
{
	namespace Core
	{
		Mesh* Mesh::Create(std::vector<float>& aVertices, bool a3D)
		{
			return new WindowsMesh(aVertices, a3D);
		}

		WindowsMesh::WindowsMesh(std::vector<float>& aVertices, bool a3D)
		{
			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			m_VAOS.push_back(vao);

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			m_VBOS.push_back(vbo);
			unsigned int vboID;

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, aVertices.size() * sizeof(float), &aVertices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, (a3D ? 3 : 2), GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(0);

			m_Indices.push_back(aVertices.size() / (a3D ? 3 : 2));
		}

		WindowsMesh::~WindowsMesh()
		{
		}

		void WindowsMesh::Render()
		{
			for (unsigned int i = 0; i < m_VAOS.size(); i++)
			{
				glBindVertexArray(m_VAOS[i]);
				glDrawArrays(GL_TRIANGLES, 0, m_Indices[i]);
			}
		}
	}
}