#include "TRpch.h"
#include "Asset.h"

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

	void MeshAsset::create(Graphics::Context* gContext)
	{
		m_vao = gContext->newVAO(m_data->meshes[0]);

		// Free the data structure, it is useless after creating the graphics object
		delete m_data;
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
	}

	bool ShaderAsset::isCreated() const
	{
		return (m_data == nullptr && m_shader.valid());
	}
}