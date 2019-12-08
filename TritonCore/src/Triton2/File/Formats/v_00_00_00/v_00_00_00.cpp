#include "TRpch.h"
#include "v_00_00_00.h"

#include "Triton2/Utility/Log.h"
#include "Triton2/File/File.h"
#include "Triton2/Scene/ECS.h"

namespace Triton
{
	namespace IO
	{
		namespace Serialization
		{
			const char* c_Version_00_00_00 = "00.00.00";

			namespace v_00_00_00
			{
				const char* c_MeshType = "mesh";
				const char* c_ImageType = "image";
				const char* c_ShaderType = "shader";
				const char* c_MaterialType = "material";
				const char* c_FontType = "font";
				const char* c_AudioType = "audio";

				IOStatus loadFormat_00_00_00(cereal::BinaryInputArchive& archive, Asset*& objectToStoreIn)
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
							objectToStoreIn = new MeshAsset(name, data);
						}
						else
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
							objectToStoreIn = new ImageAsset(name, data);
						}
						else
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
							objectToStoreIn = new ShaderAsset(name, data);
						}
						else
						{
							delete data;
						}
					}
					else if (type == c_MaterialType)
					{
						MaterialData* data = new MaterialData();
						status = loadMaterialFromArchive_00_00_00(archive, data);

						if (status.status == IOStatus::IO_OK)
						{
							objectToStoreIn = new MaterialAsset(name, data);
						}
						else
						{
							delete data;
						}
					}
					else if (type == c_FontType)
					{
						FontData* data = new FontData();
						status = loadFontFromArchive_00_00_00(archive, data);

						if (status.status == IOStatus::IO_OK)
						{
							objectToStoreIn = new FontAsset(name, data);
						}
						else
						{
							delete data;
						}
					}
					else if (type == c_AudioType)
					{
						AudioData* data = new AudioData();
						status = loadAudioFromArchive_00_00_00(archive, data);

						if (status.status == IOStatus::IO_OK)
						{
							objectToStoreIn = new AudioAsset(name, data);
						}
						else
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
					else if (asset->Type == c_MaterialType)
					{
						status = saveMaterialToArchive_00_00_00(archive, (MaterialData*)asset->Data.get());
					}
					else if (asset->Type == c_FontType)
					{
						status = saveFontToArchive_00_00_00(archive, (FontData*)asset->Data.get());
					}
					else if (asset->Type == c_AudioType)
					{
						status = saveAudioToArchive_00_00_00(archive, (AudioData*)asset->Data.get());
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
						objectToStoreIn->rawData);

					// Check for errors in loading
					if (objectToStoreIn->width <= 0 || objectToStoreIn->height <= 0 || objectToStoreIn->BPP <= 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
					}

					if (objectToStoreIn->rawData.size() == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
					}

					status = imageFromByteVector(objectToStoreIn);
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
						archive(objectToStoreIn->meshes[i].DynamicBuffer);
						archive(objectToStoreIn->meshes[i].Stride);

						archive(objectToStoreIn->meshes[i].vertices);

						archive(objectToStoreIn->meshes[i].indices);
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
						objectToStoreIn->source,
						objectToStoreIn->flags);

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

				IOStatus loadMaterialFromArchive_00_00_00(cereal::BinaryInputArchive & archive, MaterialData* objectToStoreIn)
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

					archive(objectToStoreIn->Textures,
						objectToStoreIn->Shader);

					// Check for errors in loading
					if (objectToStoreIn->Textures.size() < 1)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (objectToStoreIn->Shader == "" ||
						objectToStoreIn->Shader.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					return status;
				}

				IOStatus loadFontFromArchive_00_00_00(cereal::BinaryInputArchive & archive, FontData* objectToStoreIn)
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
					size_t metricsCount = 0;

					archive(objectToStoreIn->Buffer,
						objectToStoreIn->Width,
						objectToStoreIn->Height,
						objectToStoreIn->PWidth,
						objectToStoreIn->PHeight,
						metricsCount);

					for (size_t i = 0; i < metricsCount; i++) 
					{
						char c;
						FontData::CharMetrics metric;
						archive(c);
						archive(metric.Start, metric.End, metric.Offset, metric.Advance);
						objectToStoreIn->Metrics[c] = metric;
					}

					if (objectToStoreIn->Width == 0 || objectToStoreIn->Height == 0 || objectToStoreIn->PHeight == 0 || objectToStoreIn->PWidth == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (objectToStoreIn->Buffer.size() == 0 || objectToStoreIn->Metrics.size() == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					return status;
				}

				IOStatus loadAudioFromArchive_00_00_00(cereal::BinaryInputArchive & archive, AudioData* objectToStoreIn)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					archive(objectToStoreIn->FormatString,
						objectToStoreIn->SampleRate,
						objectToStoreIn->BitsPerSample,
						objectToStoreIn->Channels,
						objectToStoreIn->BlockAllign,
						objectToStoreIn->BytesPerSecond,
						objectToStoreIn->Format,
						objectToStoreIn->Data);

					if (objectToStoreIn->BitsPerSample == 0 || objectToStoreIn->BlockAllign == 0 || objectToStoreIn->BytesPerSecond == 0 || objectToStoreIn->Channels == 0
						|| objectToStoreIn->SampleRate == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (objectToStoreIn->Data.size() == 0)
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
						archive(rawData->meshes[i].DynamicBuffer);
						archive(rawData->meshes[i].Stride);

						archive(rawData->meshes[i].vertices);

						archive(rawData->meshes[i].indices);
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

					if (rawData->rawData.size() == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					archive(rawData->width,
						rawData->height,
						rawData->BPP,
						rawData->rawData);

					return status;
				}

				IOStatus saveShaderToArchive_00_00_00(cereal::BinaryOutputArchive & archive, ShaderData* rawData)
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
						rawData->source,
						rawData->flags);

					return status;
				}
				
				IOStatus saveMaterialToArchive_00_00_00(cereal::BinaryOutputArchive& archive, MaterialData* rawData)
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
					if (rawData->Textures.size() < 1)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (rawData->Shader == "" ||
						rawData->Shader.find_first_not_of(' ') == std::string::npos)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					archive(rawData->Textures,
						rawData->Shader);

					return status;
				}

				IOStatus saveFontToArchive_00_00_00(cereal::BinaryOutputArchive & archive, FontData* rawData)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					if (rawData->Width == 0 || rawData->Height == 0 || rawData->PHeight == 0 || rawData->PWidth == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (rawData->Buffer.size() == 0 || rawData->Metrics.size() == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					archive(rawData->Buffer,
						rawData->Width,
						rawData->Height,
						rawData->PWidth,
						rawData->PHeight,
						rawData->Metrics.size());

					for (auto& metric : rawData->Metrics) 
					{
						archive(metric.first);
						archive(metric.second.Start, metric.second.End, metric.second.Offset, metric.second.Advance);
					}

					return status;
				}

				IOStatus saveAudioToArchive_00_00_00(cereal::BinaryOutputArchive & archive, AudioData* rawData)
				{
					// The function status
					IOStatus status;
					status.status = IOStatus::IO_OK;

					if (rawData->BitsPerSample == 0 || rawData->BlockAllign == 0 || rawData->BytesPerSecond == 0 || rawData->Channels == 0
						|| rawData->SampleRate == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					if (rawData->Data.size() == 0)
					{
						status.status = IOStatus::IO_INCORRECT_FORMAT;
						return status;
					}

					archive(rawData->FormatString,
						rawData->SampleRate,
						rawData->BitsPerSample,
						rawData->Channels,
						rawData->BlockAllign,
						rawData->BytesPerSecond,
						rawData->Format,
						rawData->Data);

					return status;
				}
			}
		}
	}
}