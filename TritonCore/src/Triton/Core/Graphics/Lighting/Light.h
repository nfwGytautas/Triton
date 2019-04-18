#pragma once
#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Graphics
	{

		class Light
		{
		public:
			Light();
			virtual ~Light() { }

			void SetColor(Vector3 aColor)
			{
				mColor = aColor;
			}
			void SetPosition(Vector3 aPosition)
			{
				mPosition = aPosition;
			}
		private:
			Vector3 mColor;
			Vector3 mPosition;
		};

	}
}