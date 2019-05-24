#include "TRpch.h"
#include "Mesh.h"

void Triton::Data::Mesh::enable()
{
	object()->enable();
}

void Triton::Data::Mesh::destroyFunc(relay_ptr<PType::Context> context, PType::FactoryDestroyParams* params)
{
	context->factory->destroyObject(object(), params);
}

Triton::Data::Mesh::Mesh(PType::VAO* obj)
	: PlatformWraper(obj)
{
}
