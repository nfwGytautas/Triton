#pragma once
#include "Light.h"

namespace Triton
{
	namespace Graphics
	{

		class SpotLight : public Light
		{
		public:
			SpotLight(Vector3 aPosition, Vector3 aDirection);
			SpotLight(Vector3 aPosition, Vector3 aDirection, float aCutOff, float aOuterCutOff);
			virtual ~SpotLight() { }

			// Inherited via Light
			virtual void bind(reference<PType::Shader>& shader) override;
		private:
			Vector3 m_Position;
			Vector3 m_Direction;

			float m_CutOff;
			float m_OuterCutOff;
		};

	}
}