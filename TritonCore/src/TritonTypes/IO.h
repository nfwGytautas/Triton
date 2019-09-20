#pragma once

#include "TritonPlatform2/mathematical.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

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

			/**
			 * The enum containing some extra flags that might be set during loading
			 */
			enum AdditionalFlags
			{
				/// The file that was being loaded is outdated
				IO_OUTDATED = 1 << 0
			};

			/// Flags set
			int additionalFlags = 0;

			/// A detailed information of why the operation failed could be empty if the function was successful
			std::string additionalInformation;
		};

		/**
		 * Struct used to store the data from IO operation
		 */
		struct ImageData
		{
			/**
			 * The width of the loaded image
			 * Since 00_00_00
			 */
			int width;

			/**
			 * The height of the loaded image
			 * Since 00_00_00
			 */
			int height;

			/**
			 * The bits per pixel of the loaded image
			 * Since 00_00_00
			 */
			int BPP;

			/**
			 * The buffer of the loaded image
			 * [NOT SERIALIZED]
			 * Since 00_00_00
			 */
			std::unique_ptr<unsigned char> buffer;

			/**
			 * The buffer of the contents from the image file
			 * Since 00_00_00
			 */
			std::vector<unsigned char> rawData;
		};

		class ImageArrayData
		{
		public:
			// All textures have to be same size
			int width;
			int height;
			int BPP;

			unsigned int count;
			std::vector<reference<IO::ImageData>> individualTextures;
		};

		/// Inner structure for vertex
		struct Vertex
		{
			Vector3 Vertice;
			Vector2 UV;
			Vector3 Normal;
		};

		/**
		 * Struct that contains single mesh data
		 */
		struct Mesh
		{
			/**
			 * The vector containing the vertices of the mesh
			 * Since 00_00_00
			 */
			std::vector<Vertex> vertices;

			/**
			 * The vector containing the indices of the mesh
			 * Since 00_00_00
			 */
			std::vector<unsigned int> indices;
		};

		/**
		 * Struct used to store the data from IO operation
		 */
		struct MeshData
		{
			/**
			 * The vector containing all loaded meshes
			 * Since 00_00_00
			 */
			std::vector<Mesh> meshes;
		};

		/**
		 * Struct used to store the data from IO operation
		 */
		struct ShaderData
		{
			/**
			 * The source of the shader
			 * Since 00_00_00
			 */
			std::string source;

			/**
			 * The vertex main entry function name
			 * Since 00_00_00
			 */
			std::string vertexEntry;

			/**
			 * The pixel/fragment main entry function name
			 * Since 00_00_00
			 */
			std::string pixelEntry;

			/**
			 * The flags of this shader
			 * Since 00_00_00
			 */
			Flags::ShaderFlagset flags;
		};

		/**
		 * Struct used to create a Triton MeshAsset from MeshData
		 */
		struct IntermediateAsset
		{
			/**
			 * Format of the asset
			 * Since 00_00_00
			 */
			std::string Version;
			/**
			 * Name of the asset to be created
			 * Since 00_00_00
			 */
			std::string Name;
			/**
			 * Type of the asset
			 * Since 00_00_00
			 */
			std::string Type;
			/**
			 * Raw data of the asset cast to a void*
			 * unique_ptr to make sure there wouldn't be a memory leek here
			 * since intermediate assets should be loaded by the asset manager
			 * after being saved into the file
			 * Since 00_00_00
			 */
			std::shared_ptr<void> Data;
		};


	}
}