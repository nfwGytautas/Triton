#pragma once


#include "Triton\Core\Renderer\Renderer.h"
#include "Triton\Core\Data\Mesh.h"
#include "Triton\Core\Data\Material.h"
#include "Triton\Core\Graphics\Lighting\Light.h"
#include "Triton\Core\Platform.h"
#include "Triton\File\File.h"

#include "TritonTypes\data_collection.h"

#include "TritonShell\Entity\Registry.h"
#include "TritonShell\Entity\Components\TritonComponents.h"
#include "TritonShell\Camera\Camera.h"
#include "TritonShell\Serialize\Serialize.h"

namespace Triton
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Prepare();

		virtual void Render(relay_ptr<Core::Renderer> aRenderer);
		virtual void Update(float aDelta);

	public: // Non virtual
		void UpdateProjection(Triton::Matrix44 aNewProjection);

	public: // Resource manipulation
		size_t AddResource(std::shared_ptr<Data::MeshData> aResource)
		{
			m_MeshFiles.push_back(aResource->Path);

			return m_Meshes.Add(Platform::Create(aResource));
		}
		size_t AddResource(std::shared_ptr<Data::TextureData> aResource)
		{
			m_TextureFiles.push_back(aResource->Path);

			return m_Textures.Add(Platform::Create(aResource));
		}
		size_t AddResource(std::shared_ptr<Core::ShaderData> aResource)
		{
			m_ShaderFiles.push_back(aResource);

			return m_Shaders.Add(Platform::Create(aResource));
		}
		size_t AddResource(std::shared_ptr<Data::MaterialData> aResource)
		{
			m_MaterialFiles.push_back(aResource);

			return m_Materials.Add(std::make_shared<Data::Material>(m_Shaders.Take(aResource->Shader), m_Textures.Take(aResource->Texture)));
		}

	public: // Serialization

		template <class Archive>
		void save(Archive& ar) const
		{
			ar(m_MeshFiles);
			ar(m_TextureFiles);
			ar(m_MaterialFiles);
			ar(m_ShaderFiles);
			
			m_Registry->snapshot().entities(ar).destroyed(ar)
				.component<TR_SERIALIZABLE_COMPONENTS>(ar);
		}

		template <class Archive>
		void load(Archive& ar)
		{
			ar(m_MeshFiles);
			ar(m_TextureFiles);
			ar(m_MaterialFiles);
			ar(m_ShaderFiles);

			m_Registry->loader().entities(ar).destroyed(ar)
				.component<TR_SERIALIZABLE_COMPONENTS>(ar);

			for (auto& str : m_MeshFiles)
			{
				m_Meshes.Add(Platform::Create(Data::File::ReadMesh(str)));
			}

			for (auto& str : m_TextureFiles)
			{
				m_Textures.Add(Platform::Create(Data::File::ReadTexture(str)));
			}

			for (auto& shader : m_ShaderFiles)
			{
				m_Shaders.Add(Platform::Create(shader));
			}

			for (auto& material : m_MaterialFiles)
			{
				m_Materials.Add(std::make_shared<Data::Material>(m_Shaders.Take(material->Shader), m_Textures.Take(material->Texture)));
			}
		}
	public: // Exposed members

		inline relay_ptr<ECS::Registry> Registry()
		{
			return m_Registry;
		}
		inline void ExecuteShaderCommand(size_t aShaderID, std::function<void(relay_ptr<Core::Shader> aShader)> aFunc)
		{
			auto shader = m_Shaders.Take(aShaderID);

			shader->Enable();

			aFunc(shader);

			shader->Disable();
		}

		data_collection<Data::Mesh> m_Meshes;
		data_collection<Data::Texture> m_Textures;
		data_collection<Data::Material> m_Materials;
		data_collection<Graphics::Light> m_Lights;
	protected: // Runtime
		std::unique_ptr<ECS::Registry> m_Registry;

		std::unique_ptr<Camera> m_Camera;

	private: // Pre-initialization
		std::vector<std::string> m_MeshFiles;
		std::vector<std::string> m_TextureFiles;
		std::vector<std::shared_ptr<Data::MaterialData>> m_MaterialFiles;
		std::vector<std::shared_ptr<Core::ShaderData>> m_ShaderFiles;
		data_collection<Core::Shader> m_Shaders;

	protected: // Helper
		Components::Visual m_CurrentVisual;
		void BindVisual(Components::Visual& aVisual, relay_ptr<Core::Renderer> aRenderer);
	};

}