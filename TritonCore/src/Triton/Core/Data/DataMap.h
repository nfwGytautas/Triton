#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Structures\Mesh.h"
#include "Triton\Core\Data\Structures\Texture.h"

#include "Triton\File\File.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API DataMap
		{
		public:
			DataMap();
			virtual ~DataMap();

			std::shared_ptr<Data::Mesh> RegisterMesh(Data::MeshData& mMeshData);
			std::shared_ptr<Data::Texture> RegisterTexture(Data::TextureData& mTextureData);

			std::vector<std::string> GetMeshes() const;			
			Data::Mesh& GetMesh(std::string aMeshName);

			template <class Archive>
			void save(Archive& aArchive) const
			{
				std::vector<std::string> meshpaths(m_MeshCache.size());
				std::vector<std::string> texpaths(m_TextureCache.size());

				for (auto& key : m_MeshCache)
				{
					meshpaths.push_back(key.first);
				}
				for (auto& key : m_TextureCache)
				{
					texpaths.push_back(key.first);
				}

				aArchive(meshpaths, texpaths);
			}

			template <class Archive>
			void load(Archive& aArchive)
			{
				std::vector<std::string> meshpaths(m_MeshCache.size());
				std::vector<std::string> texpaths(m_TextureCache.size());

				aArchive(meshpaths, texpaths);

				for (std::string& str : meshpaths)
				{
					if(Data::File::Valid(str)) RegisterMesh(Data::MeshData(str));
				}
				for (std::string& str : texpaths)
				{
					if (Data::File::Valid(str)) RegisterTexture(Data::TextureData(str));
				}
			}
		private:
			std::vector<std::shared_ptr<Data::Mesh>> m_Meshes;
			std::vector<std::shared_ptr<Data::Texture>> m_Textures;

			std::unordered_map<std::string, std::shared_ptr<Data::Mesh>> m_MeshCache;
			std::unordered_map<std::string, std::shared_ptr<Data::Texture>> m_TextureCache;
		};

	}
}
