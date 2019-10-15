#pragma once

#include "Triton2/Scene/Scene.h"
#include "Triton2/Scene/ECS.h"
#include "TritonTypes/Asset.h"

namespace Triton
{
	namespace Core
	{
		class AssetManager;
	}

	class Scene;

	namespace Extension
	{
		/**
		 * Struct used to store information about same state entities in order to
		 * increase performance
		 */
		struct RenderBatch
		{
			/// Mesh of this batch
			reference<MeshAsset> Mesh;

			/// Material of this batch
			reference<MaterialAsset> Material;

			/// Entities to render
			std::vector<ECS::Entity> Entities;
		};

		/**
		 * Creates batches from the specified scene using the specified asset manager
		 *
		 * @param scene Reference to the scene object
		 * @param entitiesToRender Vector containing the entities that should be rendered
		 * @param assetManager Pointer to the AssetManager
		 * @return The batches constructed
		 */
		std::vector<RenderBatch> constructBatches(reference<Scene> scene, std::vector<ECS::Entity> entitiesToRender, Core::AssetManager* assetManager);
	}
}