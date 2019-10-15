#include "TRpch.h"
#include "Graphics.h"

#include "Triton2/Scene/Scene.h"
#include "Triton2/Scene/ECS.h"
#include "Triton2/Core/AssetManager.h"

namespace Triton
{
	namespace Extension
	{
		// A hash function used to hash a pair of any kind 
		struct hash_pair {
			template <class T1, class T2>
			size_t operator()(const std::pair<T1, T2>& p) const
			{
				auto hash1 = std::hash<T1>{}(p.first);
				auto hash2 = std::hash<T2>{}(p.second);
				return hash1 ^ hash2;
			}
		};

		std::vector<RenderBatch> constructBatches(reference<Scene> scene, std::vector<ECS::Entity> entitiesToRender, Core::AssetManager* assetManager)
		{
			std::vector<RenderBatch> result;

			std::unordered_map<std::pair<std::string, std::string>, std::vector<ECS::Entity>, hash_pair> batch;

			reference<ECS::Registry> registry = scene->entities();

			for (const ECS::Entity& entity : entitiesToRender)
			{
				Components::Visual& visual = registry->get<Components::Visual>(entity);

				std::pair<std::string, std::string> pair = std::make_pair(visual.Mesh, visual.Material);

				batch[pair].push_back(entity);
			}

			for (auto& entry : batch)
			{
				RenderBatch rBatch;
				rBatch.Mesh = assetManager->getAsset(entry.first.first).as<MeshAsset>();
				rBatch.Material = assetManager->getAsset(entry.first.second).as<MaterialAsset>();
				rBatch.Entities = entry.second;
				result.push_back(rBatch);
			}


			return result;
		}
	}
}