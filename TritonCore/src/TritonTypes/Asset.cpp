#include "TRpch.h"
#include "Asset.h"

#include "Triton2/Core/AssetManager.h"
#include "Triton2/File/File.h"
#include "Triton2/Utility/Log.h"

#include "Triton2/Assert.h"

namespace Triton
{
	Asset::Asset(std::string name)
		: m_name(name)
	{
	}

	Asset::~Asset()
	{
	}

	std::string Triton::Asset::getName() const
	{
		return m_name;
	}

	void Asset::create(Graphics::Context* gContext)
	{
	}

	void Asset::create(Audio::AudioContext* aContext)
	{
	}

	void Asset::destroy()
	{
		m_shouldDestroy = true;
	}

	bool Asset::shouldDestroyed() const
	{
		return m_shouldDestroy;
	}

	void Asset::resolveDependencies(const Core::AssetManager& manager)
	{
	}

	MeshAsset::MeshAsset(std::string name, IO::MeshData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<Graphics::VAO> MeshAsset::VAO() const
	{
		return m_vao;
	}

	void MeshAsset::create(Graphics::Context* gContext)
	{
		m_vao = gContext->newVAO(m_data->meshes[0]);

		// Free the data structure, it is useless after creating the graphics object
		delete m_data;
		m_data = nullptr;
	}

	bool MeshAsset::isCreated() const
	{
		return (m_data == nullptr && m_vao.valid());
	}

	Asset::AssetType MeshAsset::type()
	{
		return AssetType::MESH;
	}

	ImageAsset::ImageAsset(std::string name, IO::ImageData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<Graphics::Texture> ImageAsset::texture() const
	{
		return m_texture;
	}

	void ImageAsset::create(Graphics::Context* gContext)
	{
		m_texture = gContext->newTexture(*m_data);

		// Free the data structure, it is useless after creating the graphics object
		delete m_data;
		m_data = nullptr;
	}

	bool ImageAsset::isCreated() const
	{
		return (m_data == nullptr && m_texture.valid());
	}

	Asset::AssetType ImageAsset::type()
	{
		return AssetType::TEXTURE;
	}

	ShaderAsset::ShaderAsset(std::string name, IO::ShaderData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<Graphics::Shader> ShaderAsset::shader() const
	{
		return m_shader;
	}

	void ShaderAsset::create(Graphics::Context* gContext)
	{
		m_shader = gContext->newShader(*m_data);

		// Free the data structure, it is useless after creating the graphics object
		delete m_data;
		m_data = nullptr;
	}

	bool ShaderAsset::isCreated() const
	{
		return (m_data == nullptr && m_shader.valid());
	}

	Asset::AssetType ShaderAsset::type()
	{
		return AssetType::SHADER;
	}

	MaterialAsset::MaterialAsset(std::string name, IO::MaterialData* data)
		: Asset(name), m_data(data)
	{
	}

	std::vector<reference<ImageAsset>> MaterialAsset::textures() const
	{
		return m_textures;
	}

	void MaterialAsset::enableTextures()
	{
		for (const reference<ImageAsset>& texture : m_textures)
		{
			texture->texture()->enable();
		}
	}

	reference<ShaderAsset> MaterialAsset::shader() const
	{
		return m_shader;
	}

	void MaterialAsset::create(Graphics::Context* gContext)
	{
		m_textureNames = m_data->Textures;
		m_shaderName = m_data->Shader;

		m_dependencies = true;

		delete m_data;
		m_data = nullptr;
	}

	void MaterialAsset::resolveDependencies(const Core::AssetManager& manager)
	{
		if (!m_dependencies)
		{
			return;
		}

		if (!manager.hasAssets(m_textureNames) || !manager.hasAsset(m_shaderName))
		{
			TR_SYSTEM_WARN("Lacking dependencies for '{0}'", this->getName());
			TR_WARN("Lacking dependencies for '{0}'", this->getName());
			return;
		}

		for (size_t i = 0; i < m_textureNames.size(); i++)
		{
			m_textures.push_back(manager.getAsset(m_textureNames[i]).as<ImageAsset>());
			m_textures.back()->texture()->Slot = i;
		}
		
		m_shader = manager.getAsset(m_shaderName).as<ShaderAsset>();

		m_dependencies = false;
	}

	Asset::AssetType MaterialAsset::type()
	{
		return AssetType::MATERIAL;
	}

	bool MaterialAsset::isCreated() const
	{
		if (m_data != nullptr)
		{
			return false;
		}

		for (const reference<ImageAsset>& texture : m_textures)
		{
			if (!texture.valid() || !texture->texture().valid())
			{
				return false;
			}
		}

		return true;
	}

	FontAsset::FontAsset(std::string name, IO::FontData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<Graphics::Texture> FontAsset::texture() const
	{
		return m_texture;
	}

	const std::unordered_map<char, IO::FontData::CharMetrics>& FontAsset::metrics() const
	{
		return m_metrics;
	}

	unsigned int FontAsset::width() const
	{
		return m_width;
	}

	unsigned int FontAsset::height() const
	{
		return m_height;
	}

	void FontAsset::create(Graphics::Context* gContext)
	{
		IO::ImageData* imageData = new IO::ImageData();
		imageData->rawData = m_data->Buffer;
		imageData->BPP = 1;
		imageData->height = m_data->Height;
		imageData->width = m_data->Width;

		m_width = m_data->Width;
		m_height = m_data->Height;

		m_texture = gContext->newTexture(*imageData);

		m_metrics = m_data->Metrics;

		delete imageData;

		// Free the data structure, it is useless after creating the graphics object
		delete m_data;
		m_data = nullptr;
	}

	bool FontAsset::isCreated() const
	{
		return (m_data == nullptr && m_texture.valid());
	}

	Asset::AssetType FontAsset::type()
	{
		return AssetType::FONT;
	}

	AudioAsset::AudioAsset(std::string name, IO::AudioData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<Audio::Audio> AudioAsset::audio() const
	{
		return m_audio;
	}

	void AudioAsset::create(Audio::AudioContext* aContext)
	{
		m_audio = aContext->newAudio(*m_data);

		// Free the data structure, it is useless after creating the graphics object
		delete m_data;
		m_data = nullptr;
	}

	bool AudioAsset::isCreated() const
	{
		return (m_data == nullptr && m_audio.valid());
	}

	Asset::AssetType AudioAsset::type()
	{
		return AssetType::AUDIO;
	}
}