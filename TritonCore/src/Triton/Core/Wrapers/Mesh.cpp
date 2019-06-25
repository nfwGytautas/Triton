#include "TRpch.h"
#include "Mesh.h"

void Triton::Data::Mesh::enable()
{
	object()->enable();
}

Triton::Data::Mesh::Mesh(reference<PType::VAO> obj)
	: PlatformWraper(obj)
{
}
