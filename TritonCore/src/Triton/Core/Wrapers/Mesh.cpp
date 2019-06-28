#include "TRpch.h"
#include "Mesh.h"

Triton::Data::Mesh::Mesh(size_t assetID, reference<PType::VAO> object)
	: Asset(assetID), VAO(object)
{
}
