#pragma once
#include "Triton\Core\Data\Structures\Mesh.h"
#include "Triton\Core\Data\Structures\Material.h"
#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Data
	{
		struct RenderBatch
		{
			std::shared_ptr<Data::Mesh> Mesh;
			std::shared_ptr<Data::Material> Material;
			std::vector<std::shared_ptr<Matrix44>> Transformations;

			RenderBatch() { }
			RenderBatch(std::shared_ptr<Data::Mesh> aMesh, std::shared_ptr<Data::Material> aMaterial, std::vector<std::shared_ptr<Matrix44>> aTransformations)
				: Mesh(aMesh), Material(aMaterial), Transformations(aTransformations) {}
		};

		struct RenderOrder
		{
			std::vector<RenderBatch>* Batches;

			RenderOrder() { }
		};
	}
}