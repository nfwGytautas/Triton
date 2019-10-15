#include "TRpch.h"
#include "Asset.h"

#include "Triton2/Core/AssetManager.h"

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

	MeshAsset::MeshAsset(std::string name, IO::MeshData* data)
		: Asset(name), m_data(data)
	{
	}

	reference<Graphics::VAO> MeshAsset::VAO() const
	{
		return m_vao;
	}

	void MeshAsset::create(Graphics::Context* gContext, std::function<reference<Asset>(const std::string&)> getAssetCallback)
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

	void ImageAsset::create(Graphics::Context* gContext, std::function<reference<Asset>(const std::string&)> getAssetCallback)
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

	void ShaderAsset::create(Graphics::Context* gContext, std::function<reference<Asset>(const std::string&)> getAssetCallback)
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

	void MaterialAsset::create(Graphics::Context* gContext, std::function<reference<Asset>(const std::string&)> getAssetCallback)
	{
		m_mainTexture = getAssetCallback(m_data->MainTexture).as<ImageAsset>();
		m_shader = getAssetCallback(m_data->Shader).as<ShaderAsset>();

		delete m_data;
		m_data = nullptr;
	}

	bool MaterialAsset::isCreated() const
	{
		return (m_data == nullptr && m_mainTexture.valid());
	}
}