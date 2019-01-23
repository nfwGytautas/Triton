#pragma once
#include "Triton\Core\Data\Structures\Mesh.h"
#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Data
	{
		struct RenderBatch
		{
			std::shared_ptr<Data::Mesh> Mesh;
			std::shared_ptr<Data::Texture> Texture;
			std::vector<std::shared_ptr<Matrix44>> Transformations;

			RenderBatch() { }
			RenderBatch(std::shared_ptr<Data::Mesh> aMesh, std::shared_ptr<Data::Texture> aTexture, std::vector<std::shared_ptr<Matrix44>> aTransformations)
				: Mesh(aMesh), Texture(aTexture), Transformations(aTransformations) {}
		};
	}
}