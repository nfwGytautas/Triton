#include "TRpch.h"
#include "Model.h"

void Triton::Core::Model::Render()
{
	for (Mesh* mesh : m_Meshes)
	{
		mesh->Render();
	}
}

void Triton::Core::Model::AddMesh(Mesh* aMesh)
{
	m_Meshes.push_back(aMesh);
}

unsigned int Triton::Core::Model::GetWidth() const
{
	return m_Width;
}

unsigned int Triton::Core::Model::GetHeight() const
{
	return m_Height;
}
