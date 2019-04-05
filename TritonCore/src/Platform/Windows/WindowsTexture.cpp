#include "TRpch.h"
#include "WindowsTexture.h"

#include <glad\glad.h>

#include "Triton\Core\Platform.h"

namespace Triton
{
	std::shared_ptr<Triton::Data::Texture> Platform::Create(std::shared_ptr<Triton::Data::TextureData>& aData)
	{
		return std::make_shared<Triton::Core::WindowsTexture>(*aData.get());
	}
}

Triton::Core::WindowsTexture::WindowsTexture(Triton::Data::TextureData& aData)
{
	m_Width = aData.Width;
	m_Height = aData.Height;
	m_Path = aData.Path;

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, aData.Buffer.get());

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_TextureID = textureID;

	glBindTexture(GL_TEXTURE_2D, 0);
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

void Triton::Core::WindowsTexture::Unbind(unsigned int aSlot)
{
	glActiveTexture(GL_TEXTURE0 + aSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
