#pragma once
#include "Light.h"

namespace Triton
{
	namespace Graphics
	{

		class DirectionalLight : public Light
		{
		public:
			DirectionalLight(Vector3 aDirection);
			virtual ~DirectionalLight() { }

			// Inherited via Light
			virtual void Bind(Core::Shader & aShader) override;
		private:
			Vector3 m_Direction;
		};

	}
}