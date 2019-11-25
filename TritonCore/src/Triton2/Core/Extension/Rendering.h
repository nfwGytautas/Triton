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

		/**
		 * [TEMPORARY]
		 * Renders the specified text to the screen
		 *
		 * @param text Text to render
		 * @param font Font name of the text
		 * @param position Location of the text on screen
		 * @param renderer Engine renderer instance
		 * @param assetManager Assets manager where the text shader can be found
		 */
		void renderText(const std::string& text, const std::string& fontName, Vector2 position, Graphics::Renderer* renderer, Core::AssetManager* assetManager);

		/**
		 * Renders the specified texture to the screen
		 *
		 * @param texture Texture to render
		 * @param position Location of the text on screen
		 * @param renderer Engine renderer instance
		 * @param assetManager Assets manager where the texture can be found
		 */
		void renderTexture(const std::string& texture, Vector2 position, Graphics::Renderer* renderer, Core::AssetManager* assetManager);

		/**
		 * Renders the current surface to a texture on the screen
		 *
		 * @param surface ID of the surface
		 * @param position Location of the text on screen
		 * @param renderer Engine renderer instance
		 * @param assetManager Assets manager where the texture can be found
		 */
		void renderSurface(unsigned int surface, Vector2 position, Graphics::Renderer* renderer, Core::AssetManager* assetManager);
	}
}