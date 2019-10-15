#include "TRpch.h"
#include "Entity.h"

#include "Triton2/Scene/Scene.h"

namespace Triton
{
	namespace Extension
	{
		std::vector<ECS::Entity> Triton::Extension::entitiesToRender(reference<Scene> scene)
		{
			std::vector<ECS::Entity> entities;

			scene->entities()->view<Triton::Components::Visual>().each([&](auto entity, auto& visual) {
				entities.push_back(entity);
			});

			return entities;
		}
	}
}