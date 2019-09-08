#pragma once

#include "TritonPlatform2/mathematical.h"
#include "TritonPlatform2/CrossTypes/Shader.h"

namespace Triton
{
	namespace IO
	{
		/**
		 * Struct used to store the data from IO operation
		 */
		struct ImageData
		{
			/// The width of the loaded image
			int width;

			/// The height of the loaded image
			int height;

			/// The bits per pixel of the loaded image
			int BPP;

			/// The buffer of the loaded image
			std::unique_ptr<unsigned char> buffer;
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

		/**
		 * Struct used to store the data from IO operation
		 */
		struct MeshData
		{
			/**
			 * Struct that contains mesh data
			 */
			struct Mesh
			{
				/// Inner structure for vertex
				struct Vertex
				{
					Vector3 Vertice;
					Vector2 UV;
					Vector3 Normal;
				};

				/// The vector containing the vertices of the mesh
				std::vector<Vertex> vertices;

				/// The vector containing the indices of the mesh
				std::vector<unsigned int> indices;
			};

			/// The vector containing all loaded meshes
			std::vector<Mesh> meshes;
		};

		/**
		 * Struct used to store the data from IO operation
		 */
		struct ShaderData
		{
			/// The path to the source of the shader file
			std::string pathToSource;

			/// The vertex main entry function name
			std::string vertexEntry;

			/// The pixel/fragment main entry function name
			std::string pixelEntry;
		};
	}
}