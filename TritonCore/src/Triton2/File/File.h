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
		 * Read all the text inside a file as binary
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus readBinaryFromDisk(const std::string& pathToFile, std::vector<unsigned char>* objectToStoreIn);

		/**
		 * Load an image file from disk and store the data inside the specified object
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The ImageData structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus loadImageFromDisk(const std::string& pathToFile, ImageData* objectToStoreIn);

		/**
		 * Load an image from raw byte vector that is found inside ImageData::rawData member
		 *
		 * @param objectToStoreIn The ImageData structure where the function should store the loaded data 
		 *  and where the rawData is found
		 * @return The status of the specified operation
		 */
		IOStatus imageFromByteVector(ImageData* objectToStoreIn);

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
		IOStatus loadAssetFromDisk(const std::string& pathToFile, Asset*& objectToStoreIn);

		/**
		 * Save an asset file to disk
		 *
		 * @param pathToFile The path to the asset file
		 * @param asset Pointer to the intermediate asset structure
		 * @return The status of the specified operation
		 */
		IOStatus saveAssetToDisk(const std::string& pathToFile, IntermediateAsset* asset);

		namespace Serialization
		{
			/**
			 * Latest file version
			 */
			extern const char* c_Version_Latest;

			namespace v_latest
			{
				extern const char* c_MeshType;
				extern const char* c_ImageType;
				extern const char* c_ShaderType;
			}
		}
	}
}