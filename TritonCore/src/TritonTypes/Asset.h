#pragma once

#include "TritonPlatform2/CrossTypes/Core/Context.h"
#include "TritonPlatform2/CrossTypes/VAO.h"
#include "TritonPlatform2/CrossTypes/Texture.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

#include "TritonPlatform2/CrossTypes/Core/AudioContext.h"
#include "TritonPlatform2/CrossTypes/Audio.h"

#include "TritonTypes/IO2.h"

namespace Triton
{
	namespace Core
	{
		class AssetManager;
	}

	/**
	 * Base class for all asset types
	 */
	class Asset
	{
	public:
		enum class AssetType
		{
			MESH = 0,
			TEXTURE = 10,
			MATERIAL = 20,
			SHADER = 30,
			FONT = 40,
			AUDIO = 50
		};
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
		virtual void create(Graphics::Context* gContext);

		/**
		 * [Internal, avoid usage]
		 * Create the asset from data with which the asset instance was created
		 * the underlying data is freed after calling this function
		 *
		 * @param aContext Audio context pointer
		 */
		virtual void create(Audio::AudioContext* aContext);

		/**
		 * Check if the asset has been created
		 *
		 * @return True if the asset has been created
		 */
		virtual bool isCreated() const = 0;

		/**
		 * Marks the asset for destruction by the asset manager
		 * On the next available synchronize call the resources will be freed and the asset invalidated
		 */
		void destroy();

		/**
		 * Check if the specific asset needs to be destroyed
		 *
		 * @return True if asset was marked for destruction, False otherwise
		 */
		bool shouldDestroyed() const;

		/**
		 * Resolves dependencies for this asset
		 * The function is always called before create()
		 *
		 * @param assets Vector of assets
		 */
		virtual void resolveDependencies(const Core::AssetManager& manager);

		/**
		 * Type of the asset
		 *
		 * @return Asset::AssetType enum representing the type of the asset
		 */
		virtual AssetType type() = 0;

	protected:
		/// Variable to keep track if the asset has loaded it's dependencies already or not
		bool m_dependencies = false;
	private:
		/// The name of the asset
		std::string m_name;

		/// Variable to keep track if the asset should be destroyed or not
		bool m_shouldDestroy = false;
	};

	/**
	 * Mesh asset
	 */
	class MeshAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and mesh data
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
		// Inherited via Asset
		virtual AssetType type() override;
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
		 * Create the asset with a name and texture data
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
		// Inherited via Asset
		virtual AssetType type() override;
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
		 * Create the asset with a name and shader data
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
		// Inherited via Asset
		virtual AssetType type() override;
	private:
		/// The constructed Shader object for this asset
		reference<Graphics::Shader> m_shader;

		/// The raw data of this object
		IO::ShaderData* m_data;
	};

	/**
	 * Material asset
	 */
	class MaterialAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and already created material
		 *
		 * @param name The name of the asset
		 * @param data The data used to create this asset when calling create method
		 */
		MaterialAsset(std::string name, IO::MaterialData* data);

		/**
		 * Get the ImageAssets associated with this material asset
		 *
		 * @return references to an ImageAsset instance
		 */
		std::vector<reference<ImageAsset>> textures() const;

		/**
		 * Enables all textures of this material
		 */
		void enableTextures();

		/**
		 * Get the ShaderAsset associated with this material asset
		 *
		 * @return reference to a ShaderAsset instance
		 */
		reference<ShaderAsset> shader() const;

		// Inherited via Asset
		virtual void create(Graphics::Context* gContext) override;
		// Inherited via Asset
		virtual bool isCreated() const;
		// Inherited via Asset
		void resolveDependencies(const Core::AssetManager& manager);
		// Inherited via Asset
		virtual AssetType type() override;
	private:
		/// Textures used by the material
		std::vector<std::string> m_textureNames;

		/// Shader name
		std::string m_shaderName;

		/// Texture references
		std::vector<reference<ImageAsset>> m_textures;

		/// Shader for this material
		reference<ShaderAsset> m_shader;

		/// The raw data of this object
		IO::MaterialData* m_data;
	};

	/**
	 * Font asset
	 */
	class FontAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and already created font data
		 *
		 * @param data The data used to create this asset when calling create method
		 */
		FontAsset(std::string name, IO::FontData* data);

		/**
		 * Get the Texture associated with this font asset
		 *
		 * @return reference to a Texture
		 */
		reference<Graphics::Texture> texture() const;

		/**
		 * Get character metrics of this font
		 *
		 * @return Unordered map of character metrics
		 */
		const std::unordered_map<char, IO::FontData::CharMetrics>& metrics() const;

		/**
		 * Get the total width of the font texture
		 *
		 * @return Width of the font texture
		 */
		unsigned int width() const;

		/**
		 * Get the total height of the font texture
		 *
		 * @return Height of the font texture
		 */
		unsigned int height() const;

		// Inherited via Asset
		virtual void create(Graphics::Context* gContext) override;
		// Inherited via Asset
		virtual bool isCreated() const;
		// Inherited via Asset
		virtual AssetType type() override;
	private:
		/// The constructed Texture object for this asset
		reference<Graphics::Texture> m_texture;

		/// The map containing specific metrics for a character
		std::unordered_map<char, IO::FontData::CharMetrics> m_metrics;

		/// Width of the texture
		unsigned int m_width;

		/// Height of the texture
		unsigned int m_height;

		/// The raw data of this object
		IO::FontData* m_data;
	};

	/**
	 * Audio asset
	 */
	class AudioAsset : public Asset
	{
	public:
		/**
		 * Create the asset with a name and already created audio data
		 *
		 * @param data The data used to create this asset when calling create method
		 */
		AudioAsset(std::string name, IO::AudioData* data);

		/**
		 * Get the Audio associated with this audio asset
		 *
		 * @return reference to audio
		 */
		reference<Audio::Audio> audio() const;

		// Inherited via Asset
		virtual void create(Audio::AudioContext* aContext) override;
		// Inherited via Asset
		virtual bool isCreated() const;
		// Inherited via Asset
		virtual AssetType type() override;
	private:
		/// The constructed Texture object for this asset
		reference<Audio::Audio> m_audio;

		/// The raw data of this object
		IO::AudioData* m_data;
	};
}