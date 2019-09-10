#pragma once
#include "Triton2\File\Serialize.h"

#include "TritonTypes\IO.h"
#include "TritonTypes\Asset.h"
#include "Triton2\Core\AssetManager.h"

namespace Triton
{
	namespace IO
	{
		/**
		 * Check if the file is valid
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @return Is the specified file valid
		 */
		bool fileValid(const std::string& pathToFile);

		/**
		 * Read all the text inside a file
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus readFileFromDisk(const std::string& pathToFile, std::string* objectToStoreIn);

		/**
		 * Load an image file from disk and store the data inside the specified object
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The ImageData structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus loadImageFromDisk(const std::string& pathToFile, ImageData* objectToStoreIn);

		/**
		 * Load a mesh file from disk and store the data inside the specified object
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The MeshData structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus loadMeshFromDisk(const std::string& pathToFile, MeshData* objectToStoreIn);

		/**
		 * Load an asset file from disk
		 *
		 * @param pathToFile The path to the asset file
		 * @param objectToStoreIn UNINITIALIZED pointer to an asset
		 * @return The status of the specified operation
		 */
		IOStatus loadAssetFromDisk(const std::string& pathToFile, Asset*& objectToStoreIn, Graphics::Context* gContext);

		/**
		 * Save an asset file to disk
		 *
		 * @param pathToFile The path to the asset file
		 * @param asset Pointer to the intermediate asset structure
		 * @return The status of the specified operation
		 */
		IOStatus saveAssetToDisk(const std::string& pathToFile, IntermediateAsset* asset);

		namespace Version
		{
			/**
			 * Latest file version
			 * Format:
			 *	'file version'
			 *	'name of the asset'
			 *	'type of the asset'
			 *	'data for the asset'
			 */
			extern const char* c_Version_Latest;

			/** 
			 * File version 00.00.00
			 * Format: 
			 *	'file version'
			 *	'name of the asset'
			 *	'type of the asset'
			 *	'data for the asset'
			 */
			extern const char* c_Version_00_00_00;

			namespace v_latest
			{
				extern const char* c_MeshType;
				extern const char* c_ImageType;
			}

			namespace v_00_00_00
			{
				extern const char* c_MeshType;
				extern const char* c_ImageType;

				/**
				 * Loads an asset whose format is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn UNINITIALIZED pointer to an asset
				 * @return The status of the specified operation
				 */
				IOStatus loadFormat_00_00_00(cereal::BinaryInputArchive& archive, Asset*& objectToStoreIn, Graphics::Context* gContext);

				/**
				 * Loads an asset whose format is 00_00_00
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
				 * @param objectToStoreIn UNINITIALIZED pointer to a ImageData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadImageFromArchive_00_00_00(cereal::BinaryInputArchive& archive, ImageData* objectToStoreIn);

				/**
				 * Load a mesh from an asset file whose version is 00_00_00
				 *
				 * @param archive The initialized archive with the stream already opened
				 * @param objectToStoreIn UNINITIALIZED pointer to a MeshData structure
				 * @return The status of the specified operation
				 */
				IOStatus loadMeshFromArchive_00_00_00(cereal::BinaryInputArchive& archive, MeshData* objectToStoreIn);

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
			}
		}
	}
}