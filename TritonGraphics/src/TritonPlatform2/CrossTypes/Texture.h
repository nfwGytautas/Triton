#pragma once

#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	namespace Graphics
	{
		class Texture
		{
		public:
			virtual ~Texture() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			unsigned int Slot = 0;

			// Returns the raw texture specifier
			// the id for OpenGL and ShaderResourceView* for DirectX
			virtual void* raw_texture() = 0;
		protected:
			Texture()
			{ }
		};

		class CubeTexture : public Texture
		{
		public:
			virtual ~CubeTexture() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual void* raw_texture() = 0;
		protected:
			CubeTexture()
			{ }
		};
	}
}