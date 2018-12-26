#include "TRpch.h"
#include "WindowsModel.h"

#include <GL\glew.h>

namespace Triton
{
	namespace Core
	{
		Mesh* Mesh::Create(std::vector<float>& aVertices, std::vector<float>& aColorData, bool a3D)
		{
			return new WindowsMesh(aVertices, aColorData, a3D);
		}

		WindowsMesh::WindowsMesh(std::vector<float>& aVertices, std::vector<float>& aColorData, bool a3D)
		{
			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			m_VAOS.push_back(vao);

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