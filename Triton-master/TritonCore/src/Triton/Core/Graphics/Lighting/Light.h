#pragma once
#include "TritonPlatform/mathematical.h"
#include "TritonPlatform/PlatformAbstraction.h"

namespace Triton
{
	namespace Graphics
	{

		class Light
		{
		public:
			Light();
			Light(Vector3 aAmbient, Vector3 aDiffuse, Vector3 aSpecular);
			virtual ~Light() { }

			virtual void bind(reference<PType::Shader>& shader) = 0;

			unsigned int Slot;

			Vector3 Ambient;
			Vector3 Diffuse;
			Vector3 Specular;
		};

	}
}