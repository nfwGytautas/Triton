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

#include <ft2build.h>
#include FT_FREETYPE_H

// Format implementations
#include "Triton2\File\Formats\v_00_00_00\v_00_00_00.h"

#include "Triton2/Limits.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

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

			objectToStoreIn->BPP = desiredBPP;

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
				const char* c_FontType = v_00_00_00::c_FontType;
				const char* c_AudioType = v_00_00_00::c_AudioType;
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

			objectToStoreIn->resize(fileSize);

			file.read((char*)objectToStoreIn->data(), fileSize);

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

		IOStatus loadMeshFromDisk(const std::string& pathToFile, MeshData* objectToStoreIn, bool calculateTangents)
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

			unsigned int flags = 0;

			// Default flags
			flags = flags | 
				aiProcess_OptimizeGraph |
				aiProcess_OptimizeMeshes |
				aiProcess_Triangulate |
				aiProcess_GenNormals |
				aiProcess_JoinIdenticalVertices;

			// If the platform is windows then additional processing is needed
			#ifdef TR_PLATFORM_WINDOWS
			flags = flags |
				aiProcess_MakeLeftHanded |
				aiProcess_FlipWindingOrder;
			#endif

			if (calculateTangents)
			{
				flags = flags |
					aiProcess_CalcTangentSpace;
			}

			const aiScene* scene = importer.ReadFile(
				pathToFile,
				flags
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
					currentMeshLocation.Stride = Triton::Limits::VAO_NO_TBN;

					if (mesh->HasTangentsAndBitangents())
					{
						currentMeshLocation.Stride = Triton::Limits::VAO_TBN;
					}

					// Reserve memory for the mesh
					currentMeshLocation.vertices.reserve(mesh->mNumVertices * currentMeshLocation.Stride);

					// Load vertices
					for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
					{
						currentMeshLocation.vertices.push_back(mesh->mVertices[verticeIdx].x);
						currentMeshLocation.vertices.push_back(mesh->mVertices[verticeIdx].y);
						currentMeshLocation.vertices.push_back(mesh->mVertices[verticeIdx].z);

						// Check if texture coordinates exist if not use default values
						if (mesh->mTextureCoords[0])
						{
							currentMeshLocation.vertices.push_back(mesh->mTextureCoords[0][verticeIdx].x);
							currentMeshLocation.vertices.push_back(mesh->mTextureCoords[0][verticeIdx].y);
						}
						else
						{
							currentMeshLocation.vertices.push_back(0.0f);
							currentMeshLocation.vertices.push_back(0.0f);
						}
						
						// Check if normals exist if not use default values
						if (mesh->mNormals)
						{
							currentMeshLocation.vertices.push_back(mesh->mNormals[verticeIdx].x);
							currentMeshLocation.vertices.push_back(mesh->mNormals[verticeIdx].y);
							currentMeshLocation.vertices.push_back(mesh->mNormals[verticeIdx].z);
						}
						else
						{
							currentMeshLocation.vertices.push_back(1.0f);
							currentMeshLocation.vertices.push_back(1.0f);
							currentMeshLocation.vertices.push_back(1.0f);
						}

						// Check if tangents exist
						if (mesh->mTangents && currentMeshLocation.Stride == Triton::Limits::VAO_TBN)
						{
							currentMeshLocation.vertices.push_back(mesh->mTangents[verticeIdx].x);
							currentMeshLocation.vertices.push_back(mesh->mTangents[verticeIdx].y);
							currentMeshLocation.vertices.push_back(mesh->mTangents[verticeIdx].z);
						}

						// Check if by-tangent exist
						if (mesh->mBitangents && currentMeshLocation.Stride == Triton::Limits::VAO_TBN)
						{
							currentMeshLocation.vertices.push_back(mesh->mBitangents[verticeIdx].x);
							currentMeshLocation.vertices.push_back(mesh->mBitangents[verticeIdx].y);
							currentMeshLocation.vertices.push_back(mesh->mBitangents[verticeIdx].z);
						}
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

					// Static model
					currentMeshLocation.DynamicBuffer = 0;
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

		IOStatus loadFontFromDisk(const std::string& pathToFile, FontData* objectToStoreIn)
		{
			objectToStoreIn->PWidth = 0;
			objectToStoreIn->PHeight = 32;

			objectToStoreIn->Width = 128;
			objectToStoreIn->Height = 1;

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


			FT_Library  library;   /* handle to library     */
			FT_Face     face;      /* handle to face object */
			FT_Error	error;

			error = FT_Init_FreeType(&library);
			if (error) 
			{ 
				TR_SYSTEM_ERROR("FreeFont failed to initialize");

				FT_Done_FreeType(library);

				status.status = IOStatus::IO_OTHER;
				status.additionalInformation = "FreeType couldn't be initialized";
				return status;
			}

			error = FT_New_Face(library,
				pathToFile.c_str(),
				0,
				&face);
			if (error == FT_Err_Unknown_File_Format)
			{
				FT_Done_Face(face);
				FT_Done_FreeType(library);

				status.status = IOStatus::IO_INCORRECT_FORMAT;
				status.additionalInformation = "The font file could be opened and read, but it appears that its font format is unsupported";
				return status;
			}
			else if (error)
			{
				FT_Done_Face(face);
				FT_Done_FreeType(library);

				status.status = IOStatus::IO_INCORRECT_FORMAT;
				status.additionalInformation = "The font file could not be opened and read";
				return status;
			}

			error = FT_Set_Char_Size(
				face,    /* handle to face object           */
				objectToStoreIn->PWidth,       /* char_width in 1/64th of points  */
				objectToStoreIn->PHeight * 64,   /* char_height in 1/64th of points */
				96,     /* horizontal device resolution    */
				96);   /* vertical device resolution      */

			//error = FT_Set_Pixel_Sizes(
			//	face,   /* handle to face object */
			//	objectToStoreIn->PWidth,      /* pixel_width           */
			//	objectToStoreIn->PHeight);   /* pixel_height          */

			
			if (objectToStoreIn->PWidth == 0) 
			{
				objectToStoreIn->PWidth = objectToStoreIn->PHeight;
			}
			else if (objectToStoreIn->PHeight == 0)
			{
				objectToStoreIn->PHeight = objectToStoreIn->PWidth;
			}

			int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(128));
			int tex_width = 1;
			while (tex_width < max_dim) tex_width <<= 1;
			int tex_height = tex_width;

			objectToStoreIn->Width = tex_width;
			objectToStoreIn->Height = tex_height;

			objectToStoreIn->Buffer = std::vector<unsigned char>(tex_width * tex_height, 0);

			int off = 0;

			int pen_x = 0, pen_y = 0;

			for (int i = 0; i < 128; ++i) 
			{
				objectToStoreIn->Metrics[i] = {};

				FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
				FT_Bitmap* bmp = &face->glyph->bitmap;

				if (pen_x + bmp->width >= objectToStoreIn->Width) {
					pen_x = 0;
					pen_y += ((face->size->metrics.height >> 6) + 1);
				}

				for (int row = 0; row < bmp->rows; ++row) {
					for (int col = 0; col < bmp->width; ++col) {
						int x = pen_x + col;
						int y = pen_y + row;
						objectToStoreIn->Buffer[y * objectToStoreIn->Width + x] = bmp->buffer[row * bmp->pitch + col];
					}
				}

				objectToStoreIn->Metrics[i].Start.x = pen_x;
				objectToStoreIn->Metrics[i].Start.y = pen_y;
				objectToStoreIn->Metrics[i].End.x = pen_x + bmp->width;
				objectToStoreIn->Metrics[i].End.y = pen_y + bmp->rows;

				objectToStoreIn->Metrics[i].Offset.x = face->glyph->bitmap_left;
				objectToStoreIn->Metrics[i].Offset.y = face->glyph->bitmap_top;
				objectToStoreIn->Metrics[i].Advance = face->glyph->advance.x >> 6;

				pen_x += bmp->width + 1;
			}

			FT_Done_Face(face);
			FT_Done_FreeType(library);

			return status;
		}

		IOStatus loadAudioFromDisk(const std::string& pathToFile, AudioData* objectToStoreIn)
		{
			// The type of the audio file
			int fileType = -1;

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

			// Parse the type of audio file
			// Currently it parses using the extension of the file
			std::string extension = std::filesystem::path(pathToFile).extension().string();

			if (extension == ".wav")
			{
				fileType = 0;
			}

			// UNKNOWN
			if (fileType == -1)
			{
				status.status = IOStatus::IO_INCORRECT_FORMAT;
				status.additionalInformation = "Unsupported audio file type";
				return status;
			}

			// WAV
			if (fileType == 0)
			{
				struct WaveHeaderType
				{
					char chunkId[4];
					unsigned long chunkSize;
					char format[4];
					char subChunkId[4];
					unsigned long subChunkSize;
					unsigned short audioFormat;
					unsigned short numChannels;
					unsigned long sampleRate;
					unsigned long bytesPerSecond;
					unsigned short blockAlign;
					unsigned short bitsPerSample;
					char dataChunkId[4];
					unsigned long dataSize;
				};

				int error;
				FILE* filePtr;
				unsigned int count;
				WaveHeaderType waveFileHeader;
				
				// Open the wave file in binary.
				error = fopen_s(&filePtr, pathToFile.c_str(), "rb");
				if (error != 0)
				{
					status.status = IOStatus::IO_BAD_PATH;
					return status;
				}

				// Read in the wave file header.
				count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
				if (count != 1)
				{
					status.status = IOStatus::IO_INCORRECT_FORMAT;
					status.additionalInformation = "Wave file header is corrupted or in incorrect format";
					return status;
				}

				objectToStoreIn->FormatString = "wav";
				objectToStoreIn->SampleRate = waveFileHeader.sampleRate;
				objectToStoreIn->BitsPerSample = waveFileHeader.bitsPerSample;
				objectToStoreIn->Channels = waveFileHeader.numChannels;
				objectToStoreIn->BlockAllign = waveFileHeader.blockAlign;
				objectToStoreIn->BytesPerSecond = waveFileHeader.bytesPerSecond;
				objectToStoreIn->Format = waveFileHeader.audioFormat;
				
				objectToStoreIn->Data = std::vector<unsigned char>(waveFileHeader.dataSize);

				fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

				count = fread(objectToStoreIn->Data.data(), 1, waveFileHeader.dataSize, filePtr);
				if (count != waveFileHeader.dataSize)
				{
					status.status = IOStatus::IO_INCORRECT_FORMAT;
					status.additionalInformation = "Wave file corrupted. The amount of bytes read does not match header description";
					return status;
				}

			}

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

			objectToStoreIn = new Scene();
			objectToStoreIn->deserialize(archive);

			return status;
		}

		void loadSceneFromMemory(const std::string& content, Scene*& objectToStoreIn)
		{
			// Create a cereal binary reader
			std::istringstream is(content, std::ios::binary);
			cereal::BinaryInputArchive archive(is);

			objectToStoreIn = new Scene();
			objectToStoreIn->deserialize(archive);
		}

		void saveSceneToDisk(const std::string & pathToFile, Scene* scene)
		{
			// Create a cereal binary reader
			std::ofstream os(pathToFile, std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			scene->serialize(archive);
		}
	}
}