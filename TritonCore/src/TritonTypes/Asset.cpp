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

	MeshAsset::MeshAsset(std::string name, reference<Graphics::VAO> vao)
		: Asset(name), m_vao(vao)
	{
	}

	reference<Graphics::VAO> MeshAsset::VAO() const
	{
		return m_vao;
	}

	ImageAsset::ImageAsset(std::string name, reference<Graphics::Texture> tex)
		: Asset(name), m_texture(tex)
	{
	}

	reference<Graphics::Texture> ImageAsset::texture() const
	{
		return m_texture;
	}

	ShaderAsset::ShaderAsset(std::string name, reference<Graphics::Shader> shader)
		: Asset(name), m_shader(shader)
	{
	}

	reference<Graphics::Shader> ShaderAsset::shader() const
	{
		return m_shader;
	}
}