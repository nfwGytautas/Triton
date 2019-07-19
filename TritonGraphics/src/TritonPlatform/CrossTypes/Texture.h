#pragma once

#include "TritonPlatform/mathematical.h"
#include "Factory.h"

namespace Triton
{
	namespace PType
	{

		class TextureCreateParams : public FactoryCreateParams
		{
		public:
			int width;
			int height;
			int BPP;
			std::unique_ptr<unsigned char> buffer;
		};

		class Texture : public FactoryObject
		{
		public:
			virtual ~Texture() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			unsigned int Slot = 0;
		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
		protected:
			Texture()
			{ }
		};

		class TextureArrayCreateParams : public FactoryCreateParams
		{
		public:
			// All textures have to be same size
			int width;
			int height;
			int BPP;

			unsigned int count;
			std::vector<reference<TextureCreateParams>> individualTextures;
		};

		class CubeTexture : public Texture
		{
		public:
			virtual ~CubeTexture() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
		protected:
			CubeTexture()
			{ }
		};

		class Framebuffer : public FactoryObject
		{
		public:
			virtual ~Framebuffer() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual void clear(float red, float green, float blue, float alpha) = 0;
			virtual void render() = 0;

			virtual void* getAsTexture() = 0;
		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
		protected:
			Framebuffer()
			{ }
		};
	}
}