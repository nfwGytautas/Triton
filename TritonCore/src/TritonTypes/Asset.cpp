#include "TRpch.h"
#include "Asset.h"

#include "Triton2/Core/AssetManager.h"
#include "Triton2/File/File.h"
#include "Triton2/Utility/Log.h"

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

	MaterialAsset::MaterialAsset(std::string name, IO::MaterialData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<ImageAsset> MaterialAsset::mainTexture() const
	{
		return m_mainTexture;
	}

	reference<ShaderAsset> MaterialAsset::shader() const
	{
		return m_shader;
	}

	void MaterialAsset::create(Graphics::Context* gContext)
	{
		m_mainTextureName = m_data->MainTexture;
		m_shaderName = m_data->Shader;

		delete m_data;
		m_data = nullptr;
	}

	void MaterialAsset::resolveDependencies(const Core::AssetManager& manager)
	{
		if (!manager.hasAsset(m_mainTextureName) || !manager.hasAsset(m_shaderName))
		{
			return;
		}

		m_mainTexture = manager.getAsset(m_mainTextureName).as<ImageAsset>();
		m_shader = manager.getAsset(m_shaderName).as<ShaderAsset>();
	}

	bool MaterialAsset::isCreated() const
	{
		return (m_data == nullptr && m_mainTexture.valid());
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
		return (m_data == nullptr && m_texture.valid());;
	}

	void FontAsset::resolveDependencies(const Core::AssetManager& manager)
	{
	}
}