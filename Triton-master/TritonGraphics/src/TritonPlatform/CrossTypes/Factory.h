#pragma once

#include "Base.h"
#include "TritonTypes/reference.h"

namespace Triton
{
	namespace PType
	{
		class Factory
		{
		public:
			virtual ~Factory() { }

			virtual reference<FactoryObject> createShader(FactoryCreateParams* createParams) = 0;
			virtual reference<FactoryObject> createVAO(FactoryCreateParams* createParams) = 0;
			virtual reference<FactoryObject> createTexture(FactoryCreateParams* createParams) = 0;
			virtual reference<FactoryObject> createCubeTexture(FactoryCreateParams* createParams) = 0;
			virtual reference<FactoryObject> createFramebuffer(FactoryCreateParams* createParams) = 0;
			virtual reference<FactoryObject> createBitmap(FactoryCreateParams* createParams) = 0;
		};

	}
}