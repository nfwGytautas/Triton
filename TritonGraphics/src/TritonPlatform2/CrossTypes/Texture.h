#pragma once

#include "TritonPlatform/mathematical.h"

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

		class Framebuffer
		{
		public:
			virtual ~Framebuffer() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual void clear(float red, float green, float blue, float alpha) = 0;
			virtual void render() = 0;

			virtual void* getAsTexture() = 0;
		protected:
			Framebuffer()
			{ }
		};
	}
}