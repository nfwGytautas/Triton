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
			virtual void bind(reference<PType::Shader>& shader) override;
		private:
			Vector3 m_Direction;
		};

	}
}