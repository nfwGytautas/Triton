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

namespace Triton
{
	namespace IO
	{
		namespace Version
		{
			const char* c_Version_Latest = "00.00.00";
			const char* c_Version_00_00_00 = "00.00.00";

			namespace v_00_00_00
			{
				const char* c_MeshType = "mesh";
				const char* c_ImageType = "image";
				const char* c_ShaderType = "shader";
			}

			namespace v_latest
			{
				const char* c_MeshType = v_00_00_00::c_MeshType;
				const char* c_ImageType = v_00_00_00::c_MeshType;
				const char* c_ShaderType = v_00_00_00::c_ShaderType;
			}
		}

		namespace fs = std::filesystem;

		bool fileValid(const std::string& pathToFile)
		{
			return fs::exists(pathToFile);
		}

		IOStatus readFileFromDisk(const std::string & pathToFile, std::string* objectToStoreIn)
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

		IOStatus loadAssetFromDisk(const std::string& pathToFile, Asset*& objectToStoreIn, Graphics::Context* gContext)
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

			if (fileVersion == Version::c_Version_00_00_00) 
			{
				status = Version::v_00_00_00::loadFormat_00_00_00(archive, objectToStoreIn, gContext);
			}

			// Warn the user that the file is outdated
			if (fileVersion != Version::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Loading an outdated asset! Asset file format: {0}, latest format: {1}", fileVersion, Version::c_Version_Latest);
				TR_WARN("Loading an outdated asset! Asset file format: {0}, latest format: {1}", fileVersion, Version::c_Version_Latest);

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

			// Create a cereal binary reader
			std::ofstream os(pathToFile, std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			// Check if the file version
			archive(asset->Version);

			if (asset->Version == Version::c_Version_00_00_00)
			{
				status = Version::v_00_00_00::saveFormat_00_00_00(archive, asset);
			}

			// Warn the user that the file is outdated
			if (asset->Version != Version::c_Version_Latest)
			{
				TR_SYSTEM_WARN("Save as an outdated asset! Asset file format: {0}, latest format: {1}", asset->Version, Version::c_Version_Latest);
				TR_WARN("Save as an outdated asset! Asset file format: {0}, latest format: {1}", asset->Version, Version::c_Version_Latest);

				// Set the flag that the file is outdated
				status.additionalFlags |= status.IO_OUTDATED;
			}

			return status;
		}

		namespace Version
		{
			namespace v_00_00_00
			{
				IOStatus loadFormat_00_00_00(cereal::BinaryInputArchive& archive, Asset*& objectToStoreIn, Graphics::Context* gContext)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */
					std::string name;
					std::string type;

					archive(name, type);

					if (type == c_MeshType)
					{
						MeshData* data = new MeshData();
						status = loadMeshFromArchive_00_00_00(archive, data);

						if (status.status == IOStatus::IO_OK)
						{
							// No need to check for size of meshes vector since if it's empty IO_PARTIAL_OK will be set
							// Since file format 00_00_00 doesn't support multiple meshes there can only be one
							Graphics::VAO* vao = gContext->newVAO(data->meshes[0]);
							objectToStoreIn = new MeshAsset(name, vao);
						}

						if (data)
						{
							delete data;
						}
					}
					else if (type == c_ImageType)
					{
						ImageData* data = new ImageData();
						status = loadImageFromArchive_00_00_00(archive, data);

						if (status.status == IOStatus::IO_OK)
						{
							// No need to check for size of meshes vector since if it's empty IO_PARTIAL_OK will be set
							// Since file format 00_00_00 doesn't support multiple meshes there can only be one
							Graphics::Texture* tex = gContext->newTexture(*data);
							objectToStoreIn = new ImageAsset(name, tex);
						}

						if (data)
						{
							delete data;
						}
					}
					else if (type == c_ShaderType)
					{
						ShaderData* data = new ShaderData();
						status = loadShaderFromArchive_00_00_00(archive, data);

						if (status.status == IOStatus::IO_OK)
						{
							// No need to check for size of meshes vector since if it's empty IO_PARTIAL_OK will be set
							// Since file format 00_00_00 doesn't support multiple meshes there can only be one
							Graphics::Shader* shader = gContext->newShader(*data);
							objectToStoreIn = new ShaderAsset(name, shader);
						}

						if (data)
						{
							delete data;
						}
					}
					else
					{
						// The type cannot be deduced
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						status.additionalInformation = "Type cannot be deduced!";
						TR_SYSTEM_ERROR("Type of asset: {0} cannot be deduced!", type);
						TR_ERROR("Type of asset: {0} cannot be deduced!", type);
						return status;
					}

					return status;
				}

				IOStatus saveFormat_00_00_00(cereal::BinaryOutputArchive& archive, IntermediateAsset* asset)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */
					archive(asset->Name, asset->Type);

					if (asset->Type == c_MeshType)
					{
						status = saveMeshToArchive_00_00_00(archive, (MeshData*)asset->Data.get());
					}
					else if (asset->Type == c_ImageType)
					{
						status = saveImageToArchive_00_00_00(archive, (ImageData*)asset->Data.get());
					}
					else if (asset->Type == c_ShaderType)
					{
						status = saveShaderToArchive_00_00_00(archive, (ShaderData*)asset->Data.get());
					}
					else
					{
						// The type cannot be deduced
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						status.additionalInformation = "Type cannot be deduced!";
						TR_SYSTEM_ERROR("Type of asset: {0} cannot be deduced!", asset->Type);
						TR_ERROR("Type of asset: {0} cannot be deduced!", asset->Type);
						return status;
					}

					return status;
				}

				IOStatus loadImageFromArchive_00_00_00(cereal::BinaryInputArchive& archive, ImageData* objectToStoreIn)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */
					archive(objectToStoreIn->width,
							objectToStoreIn->height,
							objectToStoreIn->BPP,
							objectToStoreIn->buffer);

					// Check for errors in loading
					if (objectToStoreIn->width <= 0 || objectToStoreIn->height <= 0 || objectToStoreIn->BPP <= 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
					}

					if (!objectToStoreIn->buffer)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
					}

					return status;
				}

				IOStatus loadMeshFromArchive_00_00_00(cereal::BinaryInputArchive& archive, MeshData* objectToStoreIn)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */

					int meshCount = 0;
					archive(meshCount);

					objectToStoreIn->meshes.resize(meshCount);

					for (int i = 0; i < meshCount; i++)
					{
						int verticeCount = 0;
						int indiceCount = 0;

						archive(verticeCount);

						objectToStoreIn->meshes[i].vertices.resize(verticeCount);

						for (int j = 0; j < verticeCount; j++)
						{
							Vertex& v = objectToStoreIn->meshes[i].vertices[j];
							archive(v.Vertice, v.UV, v.Normal);
						}

						archive(indiceCount);

						objectToStoreIn->meshes[i].indices.resize(indiceCount);

						for (int j = 0; j < indiceCount; j++)
						{
							archive(objectToStoreIn->meshes[i].indices[j]);
						}
					}

					// Check for errors in loading
					if (objectToStoreIn->meshes.size() <= 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
					}

					return status;
				}

				IOStatus loadShaderFromArchive_00_00_00(cereal::BinaryInputArchive & archive, ShaderData* objectToStoreIn)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */

					archive(objectToStoreIn->vertexEntry,
							objectToStoreIn->pixelEntry,
							objectToStoreIn->source);

					// Check for errors in loading
					if (objectToStoreIn->vertexEntry == "" ||
						objectToStoreIn->vertexEntry.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (objectToStoreIn->pixelEntry == "" ||
						objectToStoreIn->pixelEntry.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (objectToStoreIn->source == "" ||
						objectToStoreIn->source.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					return status;
				}

				IOStatus saveMeshToArchive_00_00_00(cereal::BinaryOutputArchive& archive, MeshData* rawData)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */

					// Check for errors in loading
					if (rawData->meshes.size() <= 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					int meshCount = rawData->meshes.size();
					archive(meshCount);

					for (int i = 0; i < meshCount; i++)
					{
						int verticeCount = rawData->meshes[i].vertices.size();
						int indiceCount = rawData->meshes[i].indices.size();

						archive(verticeCount);

						for (int j = 0; j < verticeCount; j++)
						{
							Vertex& v = rawData->meshes[i].vertices[j];
							archive(v.Vertice, v.UV, v.Normal);
						}

						archive(indiceCount);

						rawData->meshes[i].indices.resize(indiceCount);

						for (int j = 0; j < indiceCount; j++)
						{
							archive(rawData->meshes[i].indices[j]);
						}
					}

					return status;
				}

				IOStatus saveImageToArchive_00_00_00(cereal::BinaryOutputArchive& archive, ImageData* rawData)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */

					// Check for errors in loading
					if (rawData->width <= 0 || rawData->height <= 0 || rawData->BPP <= 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (!rawData->buffer)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					archive(rawData->width,
							rawData->height,
							rawData->BPP,
							rawData->buffer);

					return status;
				}

				IOStatus saveShaderToArchive_00_00_00(cereal::BinaryOutputArchive & archive, ShaderData * rawData)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					/**
					 * File version 00.00.00
					 * Format:
					 *	'file version'
					 *	'name of the asset'
					 *	'type of the asset'
					 *	'data for the asset'
					 */

					// Check for errors in loading
					if (rawData->vertexEntry == "" || 
						rawData->vertexEntry.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (rawData->pixelEntry == "" ||
						rawData->pixelEntry.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (rawData->source == "" ||
						rawData->source.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					archive(rawData->vertexEntry, 
							rawData->pixelEntry, 
							rawData->source);

					return status;
				}
			}
		}
	}
}