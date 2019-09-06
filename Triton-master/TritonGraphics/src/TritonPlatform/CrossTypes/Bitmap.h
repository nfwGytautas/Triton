#pragma once

#include "TritonPlatform/mathematical.h"
#include "Factory.h"

#include "Texture.h"

namespace Triton
{
	namespace PType
	{

		class BitmapCreateParams : public FactoryCreateParams
		{
		public:
			reference<Texture> texture;
			unsigned int width;
			unsigned int height;
		};

		struct BitmapVertexType
		{
			Vector3 position;
			Vector2 uv;
		};

		class Bitmap : public Renderable
		{
		public:
			virtual ~Bitmap() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual void setPosition(unsigned int newX, unsigned int newY) = 0;
			virtual unsigned int getIndiceCount() = 0;
		protected:
			// Inherited via FactoryObject
			virtual void create(FactoryCreateParams* createParams) = 0;

			Bitmap()
			{}

			reference<Texture> m_Texture;

			int prtc_screenWidth, prtc_screenHeight;
			int prtc_bitmapWidth, prtc_bitmapHeight;
			int prtc_previousPosX, prtc_previousPosY;

		};
	}
}