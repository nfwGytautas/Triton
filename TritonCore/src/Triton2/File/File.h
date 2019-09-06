#pragma once
#include "Triton\TRMacros.h"

#include "TritonTypes\IO.h"

namespace Triton
{
	namespace IO
	{
		/**
		 * The status of an IO operation
		 * This struct is returned from every IO function
		 */
		struct IOStatus
		{
			/**
			 * The enum containing all the possible states for the IO operation
			 */
			enum Status
			{
				/// The operation was successful and the result has been stored
				IO_OK,

				/// The operation failed because the specified file was in incorrect format or didn't exist
				IO_BAD_PATH,

				/// The operation failed because the specified file is not of the correct format for the IO function
				IO_INCORRECT_FORMAT,

				/// The operation was successful, but some of the loading has failed
				IO_PARTIAL_OK,
			} status;

			/// A detailed information of why the operation failed could be empty if the function was successful
			std::string additionalInformation;
		};

		/**
		 * Check if the file is valid
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @return Is the specified file valid
		 */
		bool fileValid(const std::string& pathToFile);

		/**
		 * Load an image file from disk and store the data inside the specified object
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus loadImageFromDisk(const std::string& pathToFile, ImageData* objectToStoreIn);

		/**
		 * Load a mesh file from disk and store the data inside the specified object
		 *
		 * @param pathToFile The path to the file that you want to be loaded
		 * @param objectToStoreIn The structure where the function should store the loaded data
		 * @return The status of the specified operation
		 */
		IOStatus loadMeshFromDisk(const std::string& pathToFile, MeshData* objectToStoreIn);
	}
}