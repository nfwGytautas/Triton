#include "TRpch.h"
#include "Light.h"

namespace Triton
{
	namespace Graphics
	{
		Light::Light()
			: m_Ambient(1.0f, 1.0f, 1.0f), m_Diffuse(1.0f, 1.0f, 1.0f), m_Specular(1.0f, 1.0f, 1.0f)
		{
		}

		Light::Light(Vector3 aAmbient, Vector3 aDiffuse, Vector3 aSpecular)
			: m_Ambient(aAmbient), m_Diffuse(aDiffuse), m_Specular(aSpecular)
		{
		}
	}
}