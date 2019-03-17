#include "TRpch.h"
#include "DataMap.h"

extern std::shared_ptr<Triton::Data::Mesh> Create(Triton::Data::MeshData& aData);
extern std::shared_ptr<Triton::Data::Texture> Create(Triton::Data::TextureData& aData);


#include "Triton\Logger\Log.h"
#include "Triton\Core\Math\Math.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Triton
{
	namespace Core
	{
		bool checkPath(std::string mFilePath)
		{
			struct stat buffer;
			return (stat(mFilePath.c_str(), &buffer) == 0);
		}

		bool FillData(Triton::Data::MeshData& aMeshData)
		{
			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(
				aMeshData.Path,
				aiProcess_OptimizeGraph |
				aiProcess_OptimizeMeshes |
				aiProcess_Triangulate |
				aiProcess_GenNormals |
				aiProcess_JoinIdenticalVertices
			);

			if (!scene)
			{
				TR_CORE_ERROR(importer.GetErrorString());
				TR_CORE_ASSERT(scene, importer.GetErrorString());
				return false;
			}

			for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
			{
				aiMesh* mesh = scene->mMeshes[meshIdx];

				try
				{
					aMeshData.Vertices.reserve(mesh->mNumVertices);
					for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
					{
						Triton::Data::MeshData::Vertex currentVertex;

						currentVertex.Vertice.x = mesh->mVertices[verticeIdx].x;
						currentVertex.Vertice.y = mesh->mVertices[verticeIdx].y;
						currentVertex.Vertice.z = mesh->mVertices[verticeIdx].z;

						currentVertex.Normal.x = mesh->mNormals[verticeIdx].x;
						currentVertex.Normal.y = mesh->mNormals[verticeIdx].y;
						currentVertex.Normal.z = mesh->mNormals[verticeIdx].z;

						if (mesh->mTextureCoords[0])
						{
							currentVertex.UV.x = mesh->mTextureCoords[0][verticeIdx].x;
							currentVertex.UV.y = mesh->mTextureCoords[0][verticeIdx].y;
						}
						else
						{
							currentVertex.UV = glm::vec2(0.0f, 0.0f);
						}

						aMeshData.Vertices.push_back(currentVertex);
					}

					for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
					{
						//Get the face
						aiFace face = mesh->mFaces[faceIdx];
						//Add the indices of the face to the vector
						for (unsigned int indiceIdx = 0; indiceIdx < face.mNumIndices; indiceIdx++)
						{
							aMeshData.Indices.push_back(face.mIndices[indiceIdx]);
						}
					}
				}
				catch (...)
				{
					TR_CORE_ERROR("Parsing error!");
					TR_CORE_ASSERT(0, "Parsing error!");
					return false;
				}
			}

			return true;
		}

		bool FillData(Triton::Data::TextureData& aTextureData)
		{
			#ifdef TR_PLATFORM_WINDOWS
				stbi_set_flip_vertically_on_load(1);
			#endif

			TR_CORE_ASSERT(checkPath(aTextureData.Path), "Path to image does not exist or is in inccorrect format.");

			aTextureData.Buffer = std::unique_ptr<unsigned char>(stbi_load(aTextureData.Path.c_str(), &aTextureData.Width, &aTextureData.Height, &aTextureData.BPP, 4));

			TR_CORE_ASSERT(aTextureData.Buffer, stbi_failure_reason());

			return true;
		}
	}
}


Triton::Core::DataMap::DataMap()
{
}


Triton::Core::DataMap::~DataMap()
{
}

std::shared_ptr<Triton::Data::Mesh> Triton::Core::DataMap::RegisterMesh(Triton::Data::MeshData& mMeshData)
{
	if (m_MeshCache.find(mMeshData.Path) != m_MeshCache.end())
	{
		return m_MeshCache[mMeshData.Path];
	}

	bool hasPath = (mMeshData.Path != "");
	if (hasPath)
	{
		if (!FillData(mMeshData))
		{
			TR_CORE_ERROR("Failed to load {0}", mMeshData.Path);
		}
	}

	std::shared_ptr<Data::Mesh> result = Create(mMeshData);
	m_Meshes.push_back(result);

	if (hasPath)
	{
		m_MeshCache[mMeshData.Path] = result;
	}

	return result;
}

std::shared_ptr<Triton::Data::Texture> Triton::Core::DataMap::RegisterTexture(Triton::Data::TextureData& mTextureData)
{
	if (m_TextureCache.find(mTextureData.Path) != m_TextureCache.end())
	{
		return m_TextureCache[mTextureData.Path];
	}

	bool hasPath = (mTextureData.Path != "");
	if (hasPath)
	{
		if (!FillData(mTextureData))
		{
			TR_CORE_ERROR("Failed to load {0}", mTextureData.Path);
		}
	}

	std::shared_ptr<Data::Texture> result = Create(mTextureData);
	m_Textures.push_back(result);

	if (hasPath)
	{
		m_TextureCache[mTextureData.Path] = result;
	}

	return result;
}

std::vector<std::string> Triton::Core::DataMap::GetMeshes() const
{
	std::vector<std::string> keys;
	keys.reserve(m_MeshCache.size());
	for (auto& key : m_MeshCache)
	{
		keys.push_back(key.first);
	}

	return keys;
}

Triton::Data::Mesh& Triton::Core::DataMap::GetMesh(std::string aMeshName)
{
	TR_CORE_ASSERT(m_MeshCache.find(aMeshName) != m_MeshCache.end(), "Mesh " + aMeshName + " doesn't exist or isn't loaded!");
	return *m_MeshCache[aMeshName];
}
