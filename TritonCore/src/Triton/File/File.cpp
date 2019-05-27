#include "TRpch.h"
#include "File.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void LoadMeshData(std::string& aPath, Triton::PType::VAOCreateParams* params);
void LoadTextureData(std::string& aPath, Triton::PType::TextureCreateParams* params);

std::string Triton::Data::File::ReadAllText(std::string& aPath)
{
	std::ifstream is(aPath);
	std::stringstream buffer;
	buffer << is.rdbuf();
	return buffer.str();
}

bool Triton::Data::File::Valid(std::string& aPath)
{
	struct stat buffer;
	return (stat(aPath.c_str(), &buffer) == 0);
}

void Triton::Data::File::ReadMesh(std::string aPath, Triton::PType::VAOCreateParams* params)
{
	if (!Valid(aPath))
	{
		TR_CORE_ERROR("Invalid path {0}", aPath);
	}

	LoadMeshData(aPath, params);
}

void Triton::Data::File::ReadTexture(std::string aPath, Triton::PType::TextureCreateParams* params)
{
	if (!Valid(aPath))
	{
		TR_CORE_ERROR("Invalid path {0}", aPath);
		return;
	}

	return LoadTextureData(aPath, params);
}

void LoadMeshData(std::string& aPath, Triton::PType::VAOCreateParams* params)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		aPath,
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
		return;
	}

	for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		aiMesh* mesh = scene->mMeshes[meshIdx];

		try
		{
			params->vertices.reserve(mesh->mNumVertices);
			for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
			{
				Triton::PType::VAOCreateParams::Vertex currentVertex;

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
					currentVertex.UV = Triton::Vector2(0.0f, 0.0f);
				}

				params->vertices.push_back(currentVertex);
			}

			for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
			{
				//Get the face
				aiFace face = mesh->mFaces[faceIdx];
				//Add the indices of the face to the vector
				for (unsigned int indiceIdx = 0; indiceIdx < face.mNumIndices; indiceIdx++)
				{
					params->indices.push_back(face.mIndices[indiceIdx]);
				}
			}
		}
		catch (...)
		{
			TR_CORE_ERROR("Parsing error!");
			TR_CORE_ASSERT(0, "Parsing error!");
			return;
		}
	}
}

void LoadTextureData(std::string& aPath, Triton::PType::TextureCreateParams* params)
{
#ifdef TR_PLATFORM_WINDOWS
	stbi_set_flip_vertically_on_load(1);
#endif

	params->buffer = std::unique_ptr<unsigned char>(
		stbi_load(aPath.c_str(), 
			&params->width, 
			&params->height, 
			&params->BPP,
			4));

	TR_CORE_ASSERT(params->buffer, stbi_failure_reason());
}