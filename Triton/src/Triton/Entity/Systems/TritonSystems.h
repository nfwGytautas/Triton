#pragma once
#include "Triton\Core\Data\Structures\RenderBatch.h"
#include "Triton\Entity\Components\TritonComponents.h"
#include "Triton\Entity\Registry.h"

/*
Due to the limitations of EnTT registry with DLL files
These systems need to be included as macros inside the client class
*/

namespace Triton
{
	namespace Systems
	{
		//Base system class
		class System
		{
		public:
			virtual void OnUpdate(ECS::Registry& aRegistry, float aDelta) = 0;
			virtual ~System() { }
		};

		//Batching system for optimized mesh rendering
		class BatchSystem : public System
		{

		public:
			// Inherited via System
			virtual void OnUpdate(ECS::Registry& aRegistry, float aDelta) override
			{
				m_Batches.clear();

				std::vector<std::shared_ptr<Data::Mesh>> RegisteredMeshes;
				std::vector<std::shared_ptr<Data::Material>> Materials;
				std::vector<std::vector<Components::Transform>> Transforms;

				aRegistry.view<Components::Transform, Components::MeshFilter, Components::MeshRenderer>().each([&](const auto entity, auto &transform, auto &meshFilter, auto& meshRenderer) {
					bool meshFound = false;

					for (unsigned int i = 0; i < RegisteredMeshes.size(); i++)
					{
						if (RegisteredMeshes[i] == meshFilter.Mesh && Materials[i] == meshRenderer.Material)
						{
							meshFound = true;

							Transforms[i].push_back(Components::Transform(transform));

							break;
						}
					}

					if (!meshFound)
					{
						RegisteredMeshes.push_back(meshFilter.Mesh);
						Materials.push_back(meshRenderer.Material);
						Transforms.push_back(std::vector<Components::Transform> { Components::Transform(transform) });
					}

				});

				for (unsigned int i = 0; i < RegisteredMeshes.size(); i++)
				{
					std::vector<std::shared_ptr<Matrix44>> transformations;
					for (Components::Transform transform : Transforms[i])
					{
						transformations.push_back(std::make_shared<Matrix44>(Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale)));
					}
					m_Batches.push_back(Data::RenderBatch(RegisteredMeshes[i], Materials[i], transformations));
				}
			}

			std::vector<Data::RenderBatch>& GetBatches()
			{
				return m_Batches;
			}

			~BatchSystem()
			{ }
		private:
			std::vector<Data::RenderBatch> m_Batches;
		};		
	}
}

#ifndef TR_STANDARD_SYSTEMS_DISABLE

#include <memory>

/* 
Write this define to add default triton systems to your application
Included systems depend on what defines you have selected for example

@EXAMPLE if you defined TR_DEFAULT_SYSTEM_BATCH then this macro will create default batching system otherwise you need to create one

By defining TR_STANDARD_SYSTEMS_DISABLE this macro will be removed

These systems are defined inside protected space and when the macro exits next definitions will be public
*/
#define TR_INCLUDE_STANDARD_SYSTEMS \
protected:\
std::unique_ptr<Triton::Systems::BatchSystem> prtc_BatchSystem = std::make_unique<Triton::Systems::BatchSystem>();\
public:\

#define TR_STANDARD_SYSTEMS_UPDATE(registryReference, delta) \
prtc_BatchSystem->OnUpdate(registryReference, delta);\

#define TR_INITIALIZE_STANDARD_SYSTEMS

#endif