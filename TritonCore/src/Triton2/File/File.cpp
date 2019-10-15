#include "TRpch.h"
#include "File.h"

#include <set>
#include <fstream>
#include <string>
#include <filesystem>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Triton2\Utility\Log.h"

#include "Triton2\File\Serialize.h"

// Format implementations
#include "Triton2\File\Formats\v_00_00_00\v_00_00_00.h"

namespace Triton
{
	namespace IO
	{
		IOStatus imageFromByteVector(ImageData* objectToStoreIn)
		{
			// The function status
			IOStatus status;

			// If the platform is windows make sure to flip the pixel
#ifdef TR_PLATFORM_WINDOWS
			stbi_set_flip_vertically_on_load(1);
#endif

			const int desiredBPP = 4;

			objectToStoreIn->buffer = std::unique_ptr<unsigned char>(stbi_load_from_memory(
				objectToStoreIn->rawData.data(),
				(int)objectToStoreIn->rawData.size(),
				&objectToStoreIn->width,
				&objectToStoreIn->height,
				&objectToStoreIn->BPP,
				desiredBPP));

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

		namespace Serialization
		{
			const char* c_Version_Latest = "00.00.00";

			namespace v_latest
			{
				const char* c_MeshType = v_00_00_00::c_MeshType;
				const char* c_ImageType = v_00_00_00::c_ImageType;
				const char* c_ShaderType = v_00_00_00::c_ShaderType;
				const char* c_MaterialType = v_00_00_00::c_MaterialType;
			}
		}

		namespace fs = std::filesystem;

		bool fileValid(const std::string& pathToFile)
		{
			return fs::exists(pathToFile);
		}

		IOStatus createDirectory(const std::string& pathToDir)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			fs::create_directory(pathToDir);

			return status;
		}

		IOStatus readFileFromDisk(const std::string& pathToFile, std::string* objectToStoreIn)
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

			std::ifstream file{ pathToFile };
			*objectToStoreIn = std::string({ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() });

			status.status = IOStatus::IO_OK;
			return status;
		}

		IOStatus readBinaryFromDisk(const std::string& pathToFile, std::vector<unsigned char>* objectToStoreIn)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			// Check if the path exists
			if (!fileValid(pathToFile))
			{
				// The path was incorrect
				status.status = IOStatus::IO_BAD_PATH;
				return status;
			}

			std::ifstream file(pathToFile, std::ios::binary);

			file.unsetf(std::ios::skipws);

			std::streampos fileSize;

			file.seekg(0, std::ios::end);
			fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			objectToStoreIn->reserve(fileSize);

			// read the data:
			objectToStoreIn->insert(objectToStoreIn->begin(),
				std::istream_iterator<unsigned char>(file),
				std::istream_iterator<unsigned char>());

			return status;
		}

		IOStatus readBinaryFromDisk(const std::string & pathToFile, std::string * objectToStoreIn)
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

			std::ifstream file{ pathToFile, std::ios::binary };
			*objectToStoreIn = std::string({ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() });

			status.status = IOStatus::IO_OK;
			return status;
		}

		IOStatus loadImageFromDisk(const std::string& pathToFile, ImageData* objectToStoreIn)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			// Check if the path exists
			if (!fileValid(pathToFile))
			{
				// The path was incorrect
				status.status = IOStatus::IO_BAD_PATH;
				return status;
			}

			// Load the data
			readBinaryFromDisk(pathToFile, &objectToStoreIn->rawData);

			// Create the image
			status = imageFromByteVector(objectToStoreIn);
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
			objectToStoreIn->meshes.resize(scene->mNumMeshes);
			for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
			{
				aiMesh* mesh = scene->mMeshes[meshIdx];
				Mesh& currentMeshLocation = objectToStoreIn->meshes[meshIdx];
				try
				{
					// Reserve memory for the mesh
					currentMeshLocation.vertices.reserve(mesh->mNumVertices);

					// Load vertices
					Vertex currentVertex;
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

		IOStatus loadAssetFromDisk(const std::string& pathToFile, Asset*& objectToStoreIn)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			// Check if the path exists
			if (!fileValid(pathToFile))
			{
				// The path was incorrect
				status.status = IOStatus::IO_BAD_PATH;
				return status;
			}

			// Create a cereal binary reader
			std::ifstream is(pathToFile, std::ios::binary);
			cereal::BinaryInputArchive archive(is);

			// Check if the file version
			std::string fileVersion;

			archive(fileVersion);

			if (fileVersion == Serialization::c_Version_00_00_00)
			{
				status = Serialization::v_00_00_00::loadFormat_00_00_00(archive, objectToStoreIn);
			}

			// Warn the user that the file is outdated
			if (fileVersion != Serialization::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Loading an outdated asset! Asset file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);
				TR_WARN("Loading an outdated asset! Asset file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}

		IOStatus loadAssetFromMemory(const std::string& content, Asset*& objectToStoreIn)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;
			
			// Create a cereal binary reader
			std::istringstream is(content, std::ios::binary);
			cereal::BinaryInputArchive archive(is);

			// Check if the file version
			std::string fileVersion;

			archive(fileVersion);

			if (fileVersion == Serialization::c_Version_00_00_00)
			{
				status = Serialization::v_00_00_00::loadFormat_00_00_00(archive, objectToStoreIn);
			}

			// Warn the user that the file is outdated
			if (fileVersion != Serialization::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Loading an outdated asset! Asset file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);
				TR_WARN("Loading an outdated asset! Asset file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}

		IOStatus saveAssetToDisk(const std::string& pathToFile, IntermediateAsset* asset)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			if (asset->Version == "Latest")
			{
				asset->Version = Serialization::c_Version_Latest;
			}

			// Create a cereal binary reader
			std::ofstream os(pathToFile, std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			// Check if the file version
			archive(asset->Version);

			if (asset->Version == Serialization::c_Version_00_00_00)
			{
				status = Serialization::v_00_00_00::saveFormat_00_00_00(archive, asset);
			}

			// Warn the user that the file is outdated
			if (asset->Version != Serialization::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Save as an outdated asset! Asset file format: {0}, latest format: {1}", asset->Version, Serialization::c_Version_Latest);
				TR_WARN("Save as an outdated asset! Asset file format: {0}, latest format: {1}", asset->Version, Serialization::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}
		
		IOStatus loadSceneFromDisk(const std::string& pathToFile, Scene*& objectToStoreIn)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			// Check if the path exists
			if (!fileValid(pathToFile))
			{
				// The path was incorrect
				status.status = IOStatus::IO_BAD_PATH;
				return status;
			}

			// Create a cereal binary reader
			std::ifstream is(pathToFile, std::ios::binary);
			cereal::BinaryInputArchive archive(is);

			// Check if the file version
			std::string fileVersion;

			archive(fileVersion);

			if (fileVersion == Serialization::c_Version_00_00_00)
			{
				status = Serialization::v_00_00_00::loadFormat_00_00_00(archive, objectToStoreIn);
			}

			// Warn the user that the file is outdated
			if (fileVersion != Serialization::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Loading an outdated scene! Scene file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);
				TR_WARN("Loading an outdated scene! Scene file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}

		IOStatus loadSceneFromMemory(const std::string& content, Scene*& objectToStoreIn)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			// Create a cereal binary reader
			std::istringstream is(content, std::ios::binary);
			cereal::BinaryInputArchive archive(is);

			// Check if the file version
			std::string fileVersion;

			archive(fileVersion);

			if (fileVersion == Serialization::c_Version_00_00_00)
			{
				status = Serialization::v_00_00_00::loadFormat_00_00_00(archive, objectToStoreIn);
			}

			// Warn the user that the file is outdated
			if (fileVersion != Serialization::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Loading an outdated scene! Scene file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);
				TR_WARN("Loading an outdated scene! Scene file format: {0}, latest format: {1}", fileVersion, Serialization::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}

		IOStatus saveSceneToDisk(const std::string& pathToFile, SceneData* scene)
		{
			// The function status
			IOStatus status;
			status.status = IOStatus::IO_OK;

			if (scene->Version == "Latest")
			{
				scene->Version = Serialization::c_Version_Latest;
			}

			// Create a cereal binary reader
			std::ofstream os(pathToFile, std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			// Check if the file version
			archive(scene->Version);

			if (scene->Version == Serialization::c_Version_00_00_00)
			{
				status = Serialization::v_00_00_00::saveFormat_00_00_00(archive, scene);
			}

			// Warn the user that the file is outdated
			if (scene->Version != Serialization::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Save as an outdated asset! Asset file format: {0}, latest format: {1}", scene->Version, Serialization::c_Version_Latest);
				TR_WARN("Save as an outdated asset! Asset file format: {0}, latest format: {1}", scene->Version, Serialization::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}
	}
}