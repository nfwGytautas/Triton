#pragma once

#include "TritonPlatform2/CrossTypes/VAO.h"
#include "TritonPlatform2/CrossTypes/Texture.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

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

		/**
		 * Get the Texture associated with this image asset
		 *
		 * @return reference to a Texture
		 */
		reference<Graphics::Texture> texture() const;
	private:
		/// The constructed Texture object for this asset
		reference<Graphics::Texture> m_texture;
	};

	/**
	 * Shader asset
	 */
	class ShaderAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and already created Texture
		 *
		 * @param name The name of the asset
		 * @param shader The Shader that has been created for this asset
		 */
		ShaderAsset(std::string name, reference<Graphics::Shader> shader);

		/**
		 * Get the Shader associated with this shader asset
		 *
		 * @return reference to a Shader
		 */
		reference<Graphics::Shader> shader() const;
	private:
		/// The constructed Shader object for this asset
		reference<Graphics::Shader> m_shader;
	};
}