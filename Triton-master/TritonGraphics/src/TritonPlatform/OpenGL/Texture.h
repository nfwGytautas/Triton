#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN

inline void Triton::PType::GLTexture::execute()
{
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

inline void Triton::PType::GLTexture::disable()
{
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

inline void Triton::PType::GLTexture::create(FactoryCreateParams* createParams)
{
	auto texParams = OBJECT_AS(TextureCreateParams, createParams);

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texParams->width, texParams->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texParams->buffer.get());

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_textureID = textureID;

	glBindTexture(GL_TEXTURE_2D, 0);
}

inline void Triton::PType::GLTexture::destroy(FactoryDestroyParams* destroyParams)
{
	glDeleteTextures(1, &m_textureID);
}


NAMESPACE_END