#pragma once

// This is an extension of the standard IO.h file that has extra IO data
// structures and definitions. They are separated into two files because
// IO.h is also used by TritonGraphics and extra definitions are of no use
// to it.

#include "IO.h"
#include "Triton2/Scene/ECS.h"

namespace Triton
{
	namespace IO
	{
		/**
		 * Scene data structure used for serializing and deserializing scenes
		 */
		struct SceneData
		{
			/**
			 * Format of the scene
			 * Since 00_00_00
			 */
			std::string Version;

			/**
			 * Name of the scene to be created
			 * Since 00_00_00
			 */
			std::string Name;

			/**
			 * Asset names used by the scene
			 * Since 00_00_00
			 */
			std::vector<std::string> Assets;

			/**
			 * Lights used in this scene
			 * Since 00_00_00
			 */
			Graphics::LightBuffer Lights;

			/**
			 * Scene entities
			 * Since 00_00_00
			 */
			reference<ECS::Registry> Entities;
		};

		/**
		 * Material data structure used for serializing and deserializing materials
		 */
		struct MaterialData
		{
			/**
			 * Main texture name used for the material
			 * Since 00_00_00
			 */
			std::string MainTexture;

			/**
			 * Shader name used for the material
			 * Since 00_00_00
			 */
			std::string Shader;
		};
	}
}