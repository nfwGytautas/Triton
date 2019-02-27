#include "TRpch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool checkPath(std::string mFilePath)
{
	struct stat buffer;
	return (stat(mFilePath.c_str(), &buffer) == 0);
}

void Triton::Data::TextureData::Fill(const char* aPathToImage)
{
	#ifdef TR_PLATFORM_WINDOWS
	stbi_set_flip_vertically_on_load(1);
	#endif

	TR_CORE_ASSERT(checkPath(aPathToImage), "path to image does not exist or is in inccorrect format.");

	Buffer = std::unique_ptr<unsigned char>(stbi_load(aPathToImage, &Width, &Height, &BPP, 4));

	TR_CORE_ASSERT(Buffer, stbi_failure_reason());
}