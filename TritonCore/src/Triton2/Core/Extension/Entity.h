#pragma once

#include "Triton2/Scene/Scene.h"
#include "Triton2/Scene/ECS.h"

namespace Triton
{
	class Scene;

	namespace Extension
	{
		/**
		 * Returns the entities that should be rendered from the specified scene
		 *
		 * @param scene The scene to query for
		 * @return Entities that should be rendered
		 */
		std::vector<ECS::Entity> entitiesToRender(reference<Scene> scene);
	}
}