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

			virtual void bind(relay_ptr<PType::Shader> shader) = 0;

			unsigned int Slot;
		protected:
			Vector3 m_Ambient;
			Vector3 m_Diffuse;
			Vector3 m_Specular;
		};

	}
}