#pragma once
#include "Light.h"

namespace Triton
{
	namespace Graphics
	{

		class PointLight : public Light
		{
		public:
			PointLight(Vector3 aPosition);
			PointLight(Vector3 aPosition, float aConstant, float aLinear, float aQuadratic);
			virtual ~PointLight() { }

			// Inherited via Light
			virtual void bind(reference<PType::Shader>& shader) override;
		private:
			Vector3 m_Position;

			float m_Constant;
			float m_Linear;
			float m_Quadratic;
		};

	}
}