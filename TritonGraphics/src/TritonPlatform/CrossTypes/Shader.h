#pragma once

#include "TritonTypes/mathematical.h"
#include "Factory.h"

namespace Triton
{
	namespace PType
	{
		class ShaderCreateParams : public FactoryCreateParams
		{
		public:
			std::string vertexPath;
			std::string fragmentPath;
		};

		class Shader : public FactoryObject
		{
		public:
			virtual ~Shader() { }

			virtual void setUniformInt(const std::string& aUniformName, const int& aParameter) = 0;
			virtual void setUniformFloat(const std::string& aUniformName, const float& aParameter) = 0;
			virtual void setUniformVector2(const std::string& aUniformName, const Vector2 aParameter) = 0;
			virtual void setUniformVector3(const std::string& aUniformName, const Vector3 aParameter) = 0;
			virtual void setUniformVector4(const std::string& aUniformName, const Vector4 aParameter) = 0;
			virtual void setUniformMatrix44(const std::string& aUniformName, const Matrix44& aParameter) = 0;

			virtual void enable() = 0;
			virtual void disable() = 0;

		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
			virtual void destroy(FactoryDestroyParams* destroyParams) override = 0;
		protected:
			Shader()
			{ }

		protected:
			bool enabled = false;
		};
	}
}