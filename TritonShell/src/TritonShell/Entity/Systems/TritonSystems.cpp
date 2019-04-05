#include "TritonSystems.h"

void Triton::Systems::BatchSystem::OnUpdate(Triton::ECS::Registry& aRegistry, float aDelta)
{
	std::vector<std::shared_ptr<Data::Mesh>> RegisteredMeshes;
	std::vector<std::shared_ptr<Data::Material>> Materials;
	std::vector<std::vector<Components::Transform>> Transforms;

	aRegistry.view<Components::Transform>().each(
	[&](const auto entity, auto &transform) {	
		//bool meshFound = false;
		//
		//for (unsigned int i = 0; i < RegisteredMeshes.size(); i++)
		//{
		//	if (RegisteredMeshes[i] == meshFilter.Mesh && Materials[i] == meshRenderer.Material)
		//	{
		//		meshFound = true;
		//
		//		Transforms[i].push_back(Components::Transform(transform));
		//
		//		break;
		//	}
		//}
		//
		//if (!meshFound)
		//{
		//	RegisteredMeshes.push_back(meshFilter.Mesh);
		//	Materials.push_back(meshRenderer.Material);
		//	Transforms.push_back(std::vector<Components::Transform> { Components::Transform(transform) });
		//}
	});

	for (unsigned int i = 0; i < RegisteredMeshes.size(); i++)
	{
		std::vector<std::shared_ptr<Matrix44>> transformations;
		for (Components::Transform transform : Transforms[i])
		{
			transformations.push_back(std::make_shared<Matrix44>(Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale)));
		}
		//m_Batches.push_back(Data::RenderBatch(RegisteredMeshes[i], Materials[i], transformations));
	}
}

