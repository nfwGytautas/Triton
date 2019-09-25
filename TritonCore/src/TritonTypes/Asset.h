#pragma once

#include "TritonPlatform2/CrossTypes/Core/Context.h"
#include "TritonPlatform2/CrossTypes/VAO.h"
#include "TritonPlatform2/CrossTypes/Texture.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

#include "TritonTypes/IO.h"

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

		/**
		 * [Internal, avoid usage]
		 * Create the asset from data with which the asset instance was created
		 * the underlying data is freed after calling this function
		 *
		 * @param gContext Graphics context pointer
		 */
		virtual void create(Graphics::Context* gContext) = 0;

		/**
		 * Check if the asset has been created
		 *
		 * @return True if the asset has been created
		 */
		virtual bool isCreated() const = 0;
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
		 * @param data The data used to create this asset when calling create method
		 */
		MeshAsset(std::string name, IO::MeshData* data);

		/**
		 * Get the VAO associated with this mesh asset
		 *
		 * @return reference to a VAO
		 */
		reference<Graphics::VAO> VAO() const;

		// Inherited via Asset
		virtual void create(Graphics::Context* gContext) override;
		// Inherited via Asset
		virtual bool isCreated() const;
	private:
		/// The constructed VAO for this asset
		reference<Graphics::VAO> m_vao;

		/// The raw data of this object
		IO::MeshData* m_data;
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
		 * @param data The data used to create this asset when calling create method
		 */
		ImageAsset(std::string name, IO::ImageData* data);

		/**
		 * Get the Texture associated with this image asset
		 *
		 * @return reference to a Texture
		 */
		reference<Graphics::Texture> texture() const;

		// Inherited via Asset
		virtual void create(Graphics::Context* gContext) override;
		// Inherited via Asset
		virtual bool isCreated() const;
	private:
		/// The constructed Texture object for this asset
		reference<Graphics::Texture> m_texture;

		/// The raw data of this object
		IO::ImageData* m_data;
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
		 * @param data The data used to create this asset when calling create method
		 */
		ShaderAsset(std::string name, IO::ShaderData* data);

		/**
		 * Get the Shader associated with this shader asset
		 *
		 * @return reference to a Shader
		 */
		reference<Graphics::Shader> shader() const;

		// Inherited via Asset
		virtual void create(Graphics::Context* gContext) override;
		// Inherited via Asset
		virtual bool isCreated() const;
	private:
		/// The constructed Shader object for this asset
		reference<Graphics::Shader> m_shader;

		/// The raw data of this object
		IO::ShaderData* m_data;
	};
}