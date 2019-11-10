#pragma once

#include "Triton2\File\Serialize.h"
#include "Triton2\Scene\Scene.h"

#include "TritonTypes\IO2.h"
#include "TritonTypes\Asset.h"

namespace Triton
{
	namespace IO
	{
		namespace Serialization
		{
			/**
			 * File version 00.00.00
			 * Format:
			 *	'file version'
			 *	'name of the asset'
			 *	'type of the asset'
			 *	'data for the asset'
			 * For more information look at IO structures and their fields
			 */
			extern const char* c_Version_00_00_00;

			namespace v_00_00_00
			{
				extern const char* c_MeshType;
				extern const char* c_ImageType;
				extern const char* c_ShaderType;
				extern const char* c_MaterialType;
				extern const char* c_FontType;
				extern const char* c_AudioType;

				/**
				 * Loads an asset whose format is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn UNINITIALIZED pointer to an asset
				 * @return The status of the specified operation
				 */
				IOStatus loadFormat_00_00_00(cereal::BinaryInputArchive& archive, Asset*& objectToStoreIn);

				/**
				 * Saves an asset whose format is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param asset Pointer to the intermediate asset structure
				 * @return The status of the specified operation
				 */
				IOStatus saveFormat_00_00_00(cereal::BinaryOutputArchive& archive, IntermediateAsset* asset);

				/**
				 * Load an image from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn INITIALIZED pointer to a ImageData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadImageFromArchive_00_00_00(cereal::BinaryInputArchive& archive, ImageData* objectToStoreIn);

				/**
				 * Load a mesh from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn INITIALIZED pointer to a MeshData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadMeshFromArchive_00_00_00(cereal::BinaryInputArchive& archive, MeshData* objectToStoreIn);

				/**
				 * Load a shader from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn INITIALIZED pointer to a ShaderData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadShaderFromArchive_00_00_00(cereal::BinaryInputArchive& archive, ShaderData* objectToStoreIn);

				/**
				 * Load a material from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn INITIALIZED pointer to a MaterialData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadMaterialFromArchive_00_00_00(cereal::BinaryInputArchive& archive, MaterialData* objectToStoreIn);

				/**
				 * Load a font from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn INITIALIZED pointer to a FontData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadFontFromArchive_00_00_00(cereal::BinaryInputArchive& archive, FontData* objectToStoreIn);

				/**
				 * Load audio from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn INITIALIZED pointer to a AudioData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadAudioFromArchive_00_00_00(cereal::BinaryInputArchive& archive, AudioData* objectToStoreIn);

				/**
				 * Save a mesh to a asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param rawData INITIALIZED pointer to MeshData structure
				 * @return The status of the specified operation
				 */
				IOStatus saveMeshToArchive_00_00_00(cereal::BinaryOutputArchive& archive, MeshData* rawData);

				/**
				 * Save a image to a asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param rawData INITIALIZED pointer to ImageData structure
				 * @return The status of the specified operation
				 */
				IOStatus saveImageToArchive_00_00_00(cereal::BinaryOutputArchive& archive, ImageData* rawData);

				/**
				 * Save a shader to a asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param rawData INITIALIZED pointer to ShaderData structure
				 * @return The status of the specified operation
				 */
				IOStatus saveShaderToArchive_00_00_00(cereal::BinaryOutputArchive& archive, ShaderData* rawData);

				/**
				 * Save a material to a asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param rawData INITIALIZED pointer to MaterialData structure
				 * @return The status of the specified operation
				 */
				IOStatus saveMaterialToArchive_00_00_00(cereal::BinaryOutputArchive& archive, MaterialData* rawData);

				/**
				 * Save a font to a asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param rawData INITIALIZED pointer to FontData structure
				 * @return The status of the specified operation
				 */
				IOStatus saveFontToArchive_00_00_00(cereal::BinaryOutputArchive& archive, FontData* rawData);

				/**
				 * Save audio to a asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param rawData INITIALIZED pointer to AudioData structure
				 * @return The status of the specified operation
				 */
				IOStatus saveAudioToArchive_00_00_00(cereal::BinaryOutputArchive& archive, AudioData* rawData);
			}
		}
	}
}