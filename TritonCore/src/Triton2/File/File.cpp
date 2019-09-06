#include "TRpch.h"
#include "File.h"

#include <set>
#include <fstream>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include "Triton/Config.h"
#include "Triton/Utility/Algorithm.h"
#include "Triton/Logger/Log.h"

#include <filesystem>

namespace Triton
{
	namespace IO
	{
		namespace fs = std::filesystem;

		bool fileValid(const std::string& pathToFile)
		{
			return fs::exists(pathToFile);
		}

		IOStatus loadImageFromDisk(const std::string& pathToFile, ImageData* objectToStoreIn)
		{
			// The function status
			IOStatus status;

			// Check if the path exists
			if (!fileValid(pathToFile))
			{
				// The path was incorrect
				status.status = IOStatus::IO_BAD_PATH;
				return status;
			}

			// If the platform is windows make sure to flip the pixel
#ifdef TR_PLATFORM_WINDOWS
			stbi_set_flip_vertically_on_load(1);
#endif

			// Load the data
			objectToStoreIn->buffer = std::unique_ptr<unsigned char>(
				stbi_load(pathToFile.c_str(),
					&objectToStoreIn->width,
					&objectToStoreIn->height,
					&objectToStoreIn->BPP,
					4));

			// Check if load was successful
			if (!objectToStoreIn->buffer)
			{
				// The loading has failed
				status.status = IOStatus::IO_INCORRECT_FORMAT;
				status.additionalInformation = std::string(stbi_failure_reason());
				return status;
			}

			// The function has succeeded
			status.status = IOStatus::IO_OK;
			return status;
		}

		IOStatus loadMeshFromDisk(const std::string& pathToFile, MeshData* objectToStoreIn)
		{
			// The function status
			IOStatus status;

			// Check if the path exists
			if (!fileValid(pathToFile))
			{
				// The path was incorrect
				status.status = IOStatus::IO_BAD_PATH;
				return status;
			}


			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(
				pathToFile,
				aiProcess_OptimizeGraph |
				aiProcess_OptimizeMeshes |
				aiProcess_Triangulate |
				aiProcess_GenNormals |
				aiProcess_JoinIdenticalVertices

				// If the platform is windows then additional processing is needed
#ifdef TR_PLATFORM_WINDOWS
				| aiProcess_MakeLeftHanded
				| aiProcess_FlipWindingOrder
#endif
			);

			// Check if the scene could not be loaded
			if (!scene)
			{
				status.status = IOStatus::IO_INCORRECT_FORMAT;
				status.additionalInformation = std::string(importer.GetErrorString());
				return status;
			}

			// Default status
			status.status = IOStatus::IO_OK;

			// Start filling the data
			objectToStoreIn->meshes.reserve(scene->mNumMeshes);
			for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
			{
				aiMesh* mesh = scene->mMeshes[meshIdx];
				MeshData::Mesh& currentMeshLocation = objectToStoreIn->meshes[meshIdx];
				try
				{
					// Reserve memory for the mesh
					currentMeshLocation.vertices.reserve(mesh->mNumVertices);

					// Load vertices
					MeshData::Mesh::Vertex currentVertex;
					for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
					{
						currentVertex.Vertice.x = mesh->mVertices[verticeIdx].x;
						currentVertex.Vertice.y = mesh->mVertices[verticeIdx].y;
						currentVertex.Vertice.z = mesh->mVertices[verticeIdx].z;

						// Check if normals exist if not use default values
						if (mesh->mNormals)
						{
							currentVertex.Normal.x = mesh->mNormals[verticeIdx].x;
							currentVertex.Normal.y = mesh->mNormals[verticeIdx].y;
							currentVertex.Normal.z = mesh->mNormals[verticeIdx].z;
						}
						else
						{
							currentVertex.Normal = Triton::Vector3(1.0f, 1.0f, 1.0f);
						}

						// Check if texture coordinates exist if not use default values
						if (mesh->mTextureCoords[0])
						{
							currentVertex.UV.x = mesh->mTextureCoords[0][verticeIdx].x;
							currentVertex.UV.y = mesh->mTextureCoords[0][verticeIdx].y;
						}
						else
						{
							currentVertex.UV = Triton::Vector2(0.0f, 0.0f);
						}

						currentMeshLocation.vertices.push_back(currentVertex);
					}

					// Load indices
					for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
					{
						// Get the face
						aiFace face = mesh->mFaces[faceIdx];
						// Add the indices of the face to the vector
						for (unsigned int indiceIdx = 0; indiceIdx < face.mNumIndices; indiceIdx++)
						{
							currentMeshLocation.indices.push_back(face.mIndices[indiceIdx]);
						}
					}
				}
				catch (...)
				{
					// Catch the error and try continuing the loading
					status.status = IOStatus::IO_PARTIAL_OK;
					status.additionalInformation += std::string("Mesh with index " + std::to_string(meshIdx) + " failed loading \n");
				}
			}

			// Return the status of the operation
			return status;
		}

	}
}