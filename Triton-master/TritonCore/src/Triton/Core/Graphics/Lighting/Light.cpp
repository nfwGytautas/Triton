#include "TRpch.h"
#include "Light.h"

namespace Triton
{
	namespace Graphics
	{
		Light::Light()
			: Ambient(1.0f, 1.0f, 1.0f), Diffuse(1.0f, 1.0f, 1.0f), Specular(1.0f, 1.0f, 1.0f)
		{
		}

		Light::Light(Vector3 aAmbient, Vector3 aDiffuse, Vector3 aSpecular)
			: Ambient(aAmbient), Diffuse(aDiffuse), Specular(aSpecular)
		{
		}
	}
}