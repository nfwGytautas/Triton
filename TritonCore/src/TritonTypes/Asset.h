#pragma once

#include "TritonPlatform2/CrossTypes/VAO.h"
#include "TritonPlatform2/CrossTypes/Texture.h"

namespace Triton
{
	/**
	 * Base class for all asset types
	 */
	class Asset
	{
	public:
		/**
		 * Create an asset with a given name
		 *
		 * @param name The name of the asset
		 */
		Asset(std::string name);
		virtual ~Asset();

		/**
		 * Get the name associated with this asset
		 *
		 * @return The name of the asset
		 */
		std::string getName() const;
	private:
		/// The name of the asset
		std::string m_name;
	};

	/**
	 * Mesh asset
	 */
	class MeshAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and already created VAO
		 *
		 * @param name The name of the asset
		 * @param vao The VAO that has been created for this asset
		 */
		MeshAsset(std::string name, reference<Graphics::VAO> vao);

		/**
		 * Get the VAO associated with this mesh asset
		 *
		 * @return reference to a VAO
		 */
		reference<Graphics::VAO> VAO() const;
	private:
		/// The constructed VAO for this asset
		reference<Graphics::VAO> m_vao;
	};

	/**
	 * Image asset
	 */
	class ImageAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and already created Texture
		 *
		 * @param name The name of the asset
		 * @param tex The Texture that has been created for this asset
		 */
		ImageAsset(std::string name, reference<Graphics::Texture> tex);
	private:
		/// The constructed Texture object for this asset
		reference<Graphics::Texture> m_texture;
	};
}