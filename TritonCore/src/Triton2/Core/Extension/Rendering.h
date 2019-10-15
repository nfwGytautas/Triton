#pragma once

#include "Triton2/Scene/Scene.h"

namespace Triton
{
	class Scene;

	namespace Core
	{
		class AssetManager;
	}

	namespace Graphics
	{
		class Renderer;
	}

	namespace Extension
	{
		/**
		 * Renders the specified scene with the specified renderer
		 *
		 * @param scene Reference scene to be rendered
		 * @param renderer Pointer to a graphics renderer
		 * @param assetManager Pointer to an asset manager instance
		 */
		void renderScene(reference<Scene> scene, Graphics::Renderer* renderer, Core::AssetManager* assetManager);
	}
}