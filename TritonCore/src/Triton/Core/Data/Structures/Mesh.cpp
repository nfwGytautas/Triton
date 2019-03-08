#include "TRpch.h"
#include "Mesh.h"

#include "Triton\Logger\Log.h"
#include "Triton\Core\Math\Math.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

void Triton::Data::MeshData::Fill(const char* aPathToMesh)
{
	Path = std::string(aPathToMesh);

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		aPathToMesh,
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
			Vertices.reserve(mesh->mNumVertices * 3);
			UVS.reserve(mesh->mNumVertices * 2);
			Normals.reserve(mesh->mNumVertices * 3);
			for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
			{
				Vertex currentVertex;

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

				//Vertices.push_back(currentVertex);

				Vertices.push_back(currentVertex.Vertice.x);
				Vertices.push_back(currentVertex.Vertice.y);
				Vertices.push_back(currentVertex.Vertice.z);

				UVS.push_back(currentVertex.UV.x);
				UVS.push_back(currentVertex.UV.y);

				Normals.push_back(currentVertex.Normal.x);
				Normals.push_back(currentVertex.Normal.y);
				Normals.push_back(currentVertex.Normal.z);
			}

			for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
			{
				//Get the face
				aiFace face = mesh->mFaces[faceIdx];
				//Add the indices of the face to the vector
				for (unsigned int indiceIdx = 0; indiceIdx < face.mNumIndices; indiceIdx++) { Indices.push_back(face.mIndices[indiceIdx]); }
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