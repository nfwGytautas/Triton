#include "TRpch.h"
#include "WindowsTexture.h"

#include <GL\glew.h>

std::shared_ptr<Triton::Data::Texture> Triton::Data::Texture::Create(TextureData& aData)
{
	return std::make_shared<Core::WindowsTexture>(aData.Width, aData.Height, std::move(aData.Buffer));
}

Triton::Core::WindowsTexture::WindowsTexture(unsigned int aWidth, unsigned int aHeight, std::unique_ptr<unsigned char> aBuffer)
{
	m_Width = aWidth;
	m_Height = aHeight;

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, aBuffer.get());
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_TextureID = textureID;
}

Triton::Core::WindowsTexture::~WindowsTexture()
{
	glDeleteTextures(1, &m_TextureID);
}

void Triton::Core::WindowsTexture::Bind(unsigned int aSlot)
{
	glActiveTexture(GL_TEXTURE0 + aSlot);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Triton::Core::WindowsTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
