#pragma once
#include "Triton\Core\Math\Math.h"
#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace Singleton
	{
		class State;
	}

	namespace Graphics
	{

		class Light
		{
		public:
			Light();
			Light(Vector3 aAmbient, Vector3 aDiffuse, Vector3 aSpecular);
			virtual ~Light() { }

			virtual void Bind(relay_ptr<Singleton::State> aState) = 0;
		protected:
			Vector3 m_Ambient;
			Vector3 m_Diffuse;
			Vector3 m_Specular;
		};

	}
}