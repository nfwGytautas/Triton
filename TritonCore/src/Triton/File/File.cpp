#include "TRpch.h"
#include "File.h"

#include <set>
#include <fstream>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include "Triton/Config.h"
#include "Triton/Utility/Algorithm.h"

std::unordered_map<std::string, Triton::PType::ShaderDataType> Triton::Data::File::m_typeMap = 
{ 
	{"float", Triton::PType::ShaderDataType::Float},
	{"float2", Triton::PType::ShaderDataType::Float2},
	{"float3", Triton::PType::ShaderDataType::Float3},
	{"float4", Triton::PType::ShaderDataType::Float4},
	{"matrix", Triton::PType::ShaderDataType::Mat4},
	{"int", Triton::PType::ShaderDataType::Int},
	{"int2", Triton::PType::ShaderDataType::Int2},
	{"int3", Triton::PType::ShaderDataType::Int3},
	{"int4", Triton::PType::ShaderDataType::Int4},
	{"bool", Triton::PType::ShaderDataType::Bool}
};


void LoadMeshData(std::string& aPath, Triton::PType::VAOCreateParams* params);
void LoadTextureData(std::string& aPath, Triton::PType::TextureCreateParams* params);

std::string Triton::Data::File::ReadAllText(std::string& aPath)
{
	std::ifstream is(aPath);
	std::stringstream buffer;
	buffer << is.rdbuf();
	return buffer.str();
}

bool Triton::Data::File::Valid(std::string& aPath)
{
	struct stat buffer;
	return (stat(aPath.c_str(), &buffer) == 0);
}

void Triton::Data::File::ReadMesh(std::string aPath, Triton::PType::VAOCreateParams* params)
{
	if (!Valid(aPath))
	{
		TR_ERROR("Invalid path {0}", aPath);
	}

	LoadMeshData(aPath, params);
}

void Triton::Data::File::ReadTexture(std::string aPath, Triton::PType::TextureCreateParams* params)
{
	if (!Valid(aPath))
	{
		TR_ERROR("Invalid path {0}", aPath);
		return;
	}

	return LoadTextureData(aPath, params);
}

Triton::PType::ShaderDataType Triton::Data::File::isShaderVariable(const std::string& str)
{
	if (m_typeMap.find(str) != m_typeMap.end())
	{
		return m_typeMap[str];
	}
	else
	{
		return PType::ShaderDataType::None;
	}
}

Triton::PType::VAOCreateParams* Triton::Data::File::tryLoadMesh(const std::string& path)
{
	Triton::PType::VAOCreateParams* params = new Triton::PType::VAOCreateParams();

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_JoinIdenticalVertices
#ifdef TR_PLATFORM_WINDOWS
		| aiProcess_MakeLeftHanded
		| aiProcess_FlipWindingOrder
#endif
	);

	if (!scene)
	{
		TR_ERROR(importer.GetErrorString());
		return nullptr;
	}

	for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		aiMesh* mesh = scene->mMeshes[meshIdx];

		try
		{
			params->vertices.reserve(mesh->mNumVertices);
			for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
			{
				Triton::PType::VAOCreateParams::Vertex currentVertex;

				currentVertex.Vertice.x = mesh->mVertices[verticeIdx].x;
				currentVertex.Vertice.y = mesh->mVertices[verticeIdx].y;
				currentVertex.Vertice.z = mesh->mVertices[verticeIdx].z;

				if (mesh->mNormals)
				{
					currentVertex.Normal.x = mesh->mNormals[verticeIdx].x;
					currentVertex.Normal.y = mesh->mNormals[verticeIdx].y;
					currentVertex.Normal.z = mesh->mNormals[verticeIdx].z;
				}
				else
				{
					currentVertex.Normal.x = 1.0f;
					currentVertex.Normal.y = 1.0f;
					currentVertex.Normal.z = 1.0f;
				}

				if (mesh->mTextureCoords[0])
				{
					currentVertex.UV.x = mesh->mTextureCoords[0][verticeIdx].x;
					currentVertex.UV.y = mesh->mTextureCoords[0][verticeIdx].y;
				}
				else
				{
					currentVertex.UV = Triton::Vector2(0.0f, 0.0f);
				}

				params->vertices.push_back(currentVertex);
			}

			for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
			{
				//Get the face
				aiFace face = mesh->mFaces[faceIdx];
				//Add the indices of the face to the vector
				for (unsigned int indiceIdx = 0; indiceIdx < face.mNumIndices; indiceIdx++)
				{
					params->indices.push_back(face.mIndices[indiceIdx]);
				}
			}
		}
		catch (...)
		{
			TR_ERROR("Parsing error!");
			return nullptr;
		}
	}

	return params;
}

Triton::PType::TextureCreateParams* Triton::Data::File::tryLoadTexture(const std::string& path)
{
#ifdef TR_PLATFORM_WINDOWS
	stbi_set_flip_vertically_on_load(1);
#endif

	Triton::PType::TextureCreateParams* params = new Triton::PType::TextureCreateParams();

	params->buffer = std::unique_ptr<unsigned char>(
		stbi_load(path.c_str(),
			&params->width,
			&params->height,
			&params->BPP,
			4));

	if (!(params->buffer))
	{
		return nullptr;
	}
	else
	{
		return params;
	}
}

std::string Triton::Data::File::fileNameFromPath(const std::string& path)
{
	std::string _path = path.substr(path.find_last_of("/\\") + 1);
	size_t dot_i = _path.find_last_of('.');
	return _path.substr(0, dot_i);
}

bool Triton::Data::File::readShaderStream(std::string& path, PType::BufferShaderType shaderType, std::string& entryPoint,
	std::vector<PType::ShaderBufferLayout>& buffers, std::vector<PType::ShaderInputLayout>& structs, std::string& inputStructName)
{
	// Create input stream
	std::ifstream inputStream(path);

	// Current ShaderBufferLayout variables since only one can be parsed at a given time
	std::string bufferName;
	std::string structName;
	PType::BufferUpdateType updateType;
	std::vector<PType::ShaderVariable> variables;
	std::vector<PType::ShaderInputVariable> inputVariables;

	// Some helping variables for parsing
	bool expectOpenBracket = false;
	bool expectCloseBracket = false;
	bool foundBuffer = false;
	bool foundStruct = false;

	std::string haystack;
	while (std::getline(inputStream, haystack))
	{
		// Check if line isn't empty one
		if (haystack == "")
		{
			continue;
		}

		// Split the line into entries
		std::vector<std::string> initialSplit = Utility::ssplit(haystack, ' ');

		// Check if line only contained spaces
		if (initialSplit.size() < 1)
		{
			continue;
		}

		// Check if the current line has buffer definition
		if (haystack.find(TR_SHADER_BUFFER_NOTATION) != std::string::npos)
		{
			// Buffer found
			foundBuffer = true;

			// Now tell the parser that it should expect an opening bracket

			// Second entry should be the buffer name
			bufferName = initialSplit[1];

			// Check the update type it should be the first word before '_'
			std::string bufferType = Utility::ssplit(bufferName, '_')[0];

			if (bufferType == "persistant")
			{
				updateType = PType::BufferUpdateType::PERSISTANT;
			}
			else if (bufferType == "object")
			{
				updateType = PType::BufferUpdateType::OBJECT;
			}
			else if (bufferType == "frame")
			{
				updateType = PType::BufferUpdateType::FRAME;
			}
			else
			{
				// The update type is not specified or is of wrong format
				TR_WARN("Unspecified buffer update type in '{0}' for buffer '{1}'", path, bufferName);
				updateType = PType::BufferUpdateType::ALL;
			}
		}
		else if (haystack.find(TR_SHADER_STRUCT_NOTATION) != std::string::npos)// Check if this is a struct
		{
			// Buffer found
			foundStruct = true;

			structName = initialSplit[1];
		}

		// Check if the current line is opening bracket
		if (haystack == "{")
		{
			continue;
		}

		// Check if the buffer is finished
		if (haystack == "};")
		{

			if (foundBuffer)
			{
				foundBuffer = false;

				buffers.push_back(PType::ShaderBufferLayout(bufferName, updateType, shaderType, variables));
				variables.clear();
			}
			else if (foundStruct)
			{
				foundStruct = false;

				structs.push_back(PType::ShaderInputLayout(structName, inputVariables));
				inputVariables.clear();
			}

			continue;
		}

		// Check if the line is a variable
		std::string variable = Utility::trim(initialSplit[0], " \t\n;:");

		PType::ShaderDataType type = isShaderVariable(variable);

		if (type != PType::ShaderDataType::None && foundBuffer)
		{
			std::string variable = Utility::trim(initialSplit[1], " ;\t\n");

			variables.push_back(PType::ShaderVariable(variable, type));
		}
		else if (type != PType::ShaderDataType::None && foundStruct)
		{
			// Find the struct name

			// The second string after ':' should be the layout unless there is an embedded comment
			std::string variable = Utility::ssplit(haystack, ':')[1];
			variable = Utility::trim(variable, " ;0123456789");

			inputVariables.push_back(PType::ShaderInputVariable(variable, type));
		}

		// Check if function is an entry point
		if (haystack.find(entryPoint) != std::string::npos)
		{
			// Entry point function found now check what is the input struct of this shader

			// Find the start and end indexes of the parameter list
			size_t start = haystack.find("(");
			size_t end = haystack.find(")");

			// Check if the parameters can be deduced
			if (start == std::string::npos && end == std::string::npos)
			{
				TR_ERROR("Parsing shader layout for '{0}' failed because the entry point '{1}' parameters cannot be deduced",
					path, entryPoint);
				return false;
			}

			// Get the parameters
			std::vector<std::string> functionEntries = Utility::ssplit(haystack.substr(start + 1, end - start - 1), ',');

			// Assumption that the first parameter is the input layout
			inputStructName = Utility::ssplit(functionEntries[0], ' ')[0];
		}
	}

	return true;
}

Triton::PType::ShaderLayout Triton::Data::File::ReadShaderLayout(Triton::PType::ShaderCreateParams* params)
{
	// Vectors containing the shaderbuffers
	std::vector<PType::ShaderBufferLayout> buffers;

	// Index of the definitive input struct
	size_t inputIndex = -1;

	// Vector containing all found struct
	std::vector<PType::ShaderInputLayout> structs;


	std::string entryStruct;
	// Parse vertex shader
	if (!readShaderStream(params->vertexPath, PType::BufferShaderType::VERTEX, params->entryPointVertex, buffers, structs, entryStruct))
	{
		return PType::ShaderLayout();
	}

	// Check if such struct was defined
	for (unsigned int i = 0; i < structs.size(); i++)
	{
		if (structs[i].getName() == entryStruct)
		{
			inputIndex = i;
			break;
		}
	}

	// Check if the index was found
	if (inputIndex == -1)
	{
		TR_ERROR("Parsing shader layout for '{0}' failed because '{1}' was either not defined or not found by the parser",
			params->vertexPath, entryStruct);
		return PType::ShaderLayout();
	}

	// Parse pixel shader
	if (!readShaderStream(params->fragmentPath, PType::BufferShaderType::PIXEL, params->entryPointFragment, buffers, structs, entryStruct))
	{
		return PType::ShaderLayout();
	}

	return PType::ShaderLayout(structs[inputIndex], buffers);
}

void LoadMeshData(std::string& aPath, Triton::PType::VAOCreateParams* params)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		aPath,
		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_JoinIdenticalVertices 
#ifdef TR_PLATFORM_WINDOWS
		| aiProcess_MakeLeftHanded 
		| aiProcess_FlipWindingOrder
#endif
	);

	if (!scene)
	{
		TR_ERROR(importer.GetErrorString());
		TR_CORE_ASSERT(scene, importer.GetErrorString());
		return;
	}

	for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		aiMesh* mesh = scene->mMeshes[meshIdx];

		try
		{
			params->vertices.reserve(mesh->mNumVertices);
			for (unsigned int verticeIdx = 0; verticeIdx < mesh->mNumVertices; verticeIdx++)
			{
				Triton::PType::VAOCreateParams::Vertex currentVertex;

				currentVertex.Vertice.x = mesh->mVertices[verticeIdx].x;
				currentVertex.Vertice.y = mesh->mVertices[verticeIdx].y;
				currentVertex.Vertice.z = mesh->mVertices[verticeIdx].z;

				if (mesh->mNormals)
				{
					currentVertex.Normal.x = mesh->mNormals[verticeIdx].x;
					currentVertex.Normal.y = mesh->mNormals[verticeIdx].y;
					currentVertex.Normal.z = mesh->mNormals[verticeIdx].z;
				}
				else
				{
					currentVertex.Normal.x = 1.0f;
					currentVertex.Normal.y = 1.0f;
					currentVertex.Normal.z = 1.0f;
				}	

				if (mesh->mTextureCoords[0])
				{
					currentVertex.UV.x = mesh->mTextureCoords[0][verticeIdx].x;
					currentVertex.UV.y = mesh->mTextureCoords[0][verticeIdx].y;
				}
				else
				{
					currentVertex.UV = Triton::Vector2(0.0f, 0.0f);
				}

				params->vertices.push_back(currentVertex);
			}

			for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
			{
				//Get the face
				aiFace face = mesh->mFaces[faceIdx];
				//Add the indices of the face to the vector
				for (unsigned int indiceIdx = 0; indiceIdx < face.mNumIndices; indiceIdx++)
				{
					params->indices.push_back(face.mIndices[indiceIdx]);
				}
			}
		}
		catch (...)
		{
			TR_ERROR("Parsing error!");
			TR_CORE_ASSERT(0, "Parsing error!");
			return;
		}
	}
}

void LoadTextureData(std::string& aPath, Triton::PType::TextureCreateParams* params)
{
#ifdef TR_PLATFORM_WINDOWS
	stbi_set_flip_vertically_on_load(1);
#endif

	params->buffer = std::unique_ptr<unsigned char>(
		stbi_load(aPath.c_str(), 
			&params->width, 
			&params->height, 
			&params->BPP,
			4));
	
	TR_CORE_ASSERT(params->buffer, stbi_failure_reason());
}