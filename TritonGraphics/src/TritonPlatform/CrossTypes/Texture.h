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

			unsigned int Slot;
		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
			virtual void destroy(FactoryDestroyParams* destroyParams) override = 0;
		protected:
			Texture()
			{ }
		};

	}
}