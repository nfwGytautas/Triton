#pragma once

#include "TritonPlatform2/mathematical.h"
#include "TritonPlatform2/CrossTypes/Base.h"

#include "Texture.h"

namespace Triton
{
	namespace Graphics
	{

		class BitmapCreateParams
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
			Bitmap()
			{}

			reference<Texture> m_Texture;

			int prtc_screenWidth, prtc_screenHeight;
			int prtc_bitmapWidth, prtc_bitmapHeight;
			int prtc_previousPosX, prtc_previousPosY;
		};
	}
}