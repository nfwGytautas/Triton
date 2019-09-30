#pragma once

#include "ShaderBuffers.h"
#include "TritonTypes/IO.h"

namespace Triton
{
	/**
	 * Scene object
	 */
	class Scene
	{
	public:
		/**
		 * Creates a scene with a given name
		 *
		 * @param name Name of the scene
		 */
		Scene(std::string name);

		/**
		 * Creates a IO::SceneData structure from this scene
		 *
		 * @return IO::SceneData representation of this scene
		 */
		IO::SceneData toRawData();

		/**
		 * Returns the name of the scene
		 *
		 * @return Name of the scene
		 */
		std::string getName() const;

		/**
		 * Returns the assets used by this scene
		 *
		 * @return Vector of scene assets
		 */
		std::vector<std::string>& assets();

		/**
		 * Returns the lights of this scene
		 *
		 * @return Graphics::LightBuffer structure of lights
		 */
		Graphics::LightBuffer& lights();
	private:
		/// Name of the scene
		std::string m_name;

		/// Asset names that are used in the scene
		std::vector<std::string> m_assets;

		/// Light buffer of this scene
		Graphics::LightBuffer m_lights;
	};
}