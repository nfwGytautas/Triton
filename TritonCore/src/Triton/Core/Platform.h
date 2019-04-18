#pragma once

#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{
		class Mesh;
		class Texture;

		struct MeshData;
		struct TextureData;
	}

	namespace Core
	{
		class Shader;
		class Renderer;
	}

	class Platform
	{
	public:
		static std::shared_ptr<Triton::Data::Mesh> Create(std::shared_ptr<Triton::Data::MeshData>& aData);
		static std::shared_ptr<Triton::Data::Texture> Create(std::shared_ptr<Triton::Data::TextureData>& aData);
	};
}